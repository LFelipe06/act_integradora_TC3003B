import sys
import os
import time
import subprocess
import threading
from PyQt5.QtWidgets import (
    QApplication, QWidget, QLabel, QPushButton, QVBoxLayout,
    QFileDialog, QListWidget, QMainWindow, QAction, QHBoxLayout,
    QGroupBox, QListWidgetItem, QCheckBox, QProgressBar, QSizePolicy,
    QSlider, QLineEdit
)
from PyQt5.QtGui import QPixmap, QIcon, QIntValidator
from PyQt5.QtCore import Qt, QSize, QThread, pyqtSignal, QTimer, QProcess


class ImageProcessingThread(QThread):
    progress = pyqtSignal(int)
    done = pyqtSignal()

    def __init__(self, input_folder, output_folder, selected_effects):
        super().__init__()
        self.input_folder = input_folder
        self.output_folder = output_folder
        self.selected_effects = selected_effects

    def run(self):
        files = [f for f in os.listdir(self.input_folder) if f.lower().endswith(".bmp")]
        total = len(files)
        if total == 0:
            self.progress.emit(0)
            self.done.emit()
            return

        for i, f in enumerate(files):
            time.sleep(0.05)  # Simula tiempo de procesamiento
            self.progress.emit(int((i + 1) / total * 100))

        self.done.emit()


class ImageProcessorGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Procesador de Imágenes")
        self.setGeometry(200, 200, 1000, 600)

        self.input_folder = ""
        self.output_folder = "carpeta_salida"
        self.processing_thread = None

        self.init_ui()

    def init_ui(self):
        menu_bar = self.menuBar()
        dev_menu = menu_bar.addMenu("Desarrolladores")
        for dev in ["Juan Pérez", "Ana Gómez", "Luis Martínez"]:
            dev_menu.addAction(QAction(dev, self))

        widget = QWidget()
        main_layout = QHBoxLayout()
        widget.setLayout(main_layout)
        self.setCentralWidget(widget)

        # Lado izquierdo
        left_layout = QVBoxLayout()
        self.select_input_btn = QPushButton("Seleccionar carpeta de entrada")
        self.select_input_btn.clicked.connect(self.select_input_folder)
        left_layout.addWidget(self.select_input_btn)

        self.input_label = QLabel("Carpeta de entrada: No seleccionada")
        self.output_label = QLabel(f"Carpeta de salida: {os.path.abspath(self.output_folder)}")
        left_layout.addWidget(self.input_label)
        left_layout.addWidget(self.output_label)

        self.input_list = QListWidget()
        self.setup_icon_view(self.input_list)
        input_group = QGroupBox("Miniaturas - Carpeta de entrada")
        input_group.setLayout(QVBoxLayout())
        input_group.layout().addWidget(self.input_list)
        left_layout.addWidget(input_group)

        self.output_list = QListWidget()
        self.setup_icon_view(self.output_list)
        output_group = QGroupBox("Miniaturas - Carpeta de salida")
        output_group.setLayout(QVBoxLayout())
        output_group.layout().addWidget(self.output_list)
        left_layout.addWidget(output_group)

        main_layout.addLayout(left_layout, 2)

        # Lado derecho
        right_layout = QVBoxLayout()

        effects_group = QGroupBox("Efectos a aplicar")
        effects_layout = QVBoxLayout()

        self.cb_grayscale = QCheckBox("Escala de grises")
        self.cb_flip_h = QCheckBox("Volteo horizontal")
        self.cb_flip_v = QCheckBox("Volteo vertical")
        self.cb_gray_flip_h = QCheckBox("Grises + Volteo horizontal")
        self.cb_gray_flip_v = QCheckBox("Grises + Volteo vertical")

        for cb in [self.cb_grayscale, self.cb_flip_h, self.cb_flip_v,
                   self.cb_gray_flip_h, self.cb_gray_flip_v]:
            effects_layout.addWidget(cb)

        # Desenfoque con slider y entrada
        blur_layout = QHBoxLayout()
        self.cb_blur = QCheckBox("Desenfoque")

        self.blur_slider = QSlider(Qt.Horizontal)
        self.blur_slider.setRange(55, 155)
        self.blur_slider.setValue(55)
        self.blur_slider.setFixedWidth(100)

        self.blur_input = QLineEdit("55")
        self.blur_input.setFixedWidth(40)
        self.blur_input.setMaxLength(3)
        self.blur_input.setAlignment(Qt.AlignCenter)
        self.blur_input.setValidator(QIntValidator(55, 155, self))

        self.blur_slider.valueChanged.connect(lambda val: self.blur_input.setText(str(val)))
        self.blur_input.textChanged.connect(
            lambda val: self.blur_slider.setValue(int(val)) if val.isdigit() and 55 <= int(val) <= 155 else None
        )

        blur_layout.addWidget(self.cb_blur)
        blur_layout.addWidget(self.blur_slider)
        blur_layout.addWidget(self.blur_input)
        effects_layout.addLayout(blur_layout)

        effects_group.setLayout(effects_layout)
        right_layout.addWidget(effects_group)

        self.process_btn = QPushButton("Procesar imágenes")
        self.process_btn.clicked.connect(self.start_processing)
        right_layout.addWidget(self.process_btn)

        # Barra de progreso horizontal
        self.progress_bar = QProgressBar()
        self.progress_bar.setRange(0, 100)
        self.progress_bar.setValue(0)
        self.progress_bar.setTextVisible(True)
        self.progress_bar.setOrientation(Qt.Horizontal)
        self.progress_bar.setFixedHeight(25)

        progress_group = QGroupBox("Progreso")
        progress_layout = QVBoxLayout()
        progress_layout.addWidget(self.progress_bar)
        progress_group.setLayout(progress_layout)

        right_layout.addWidget(progress_group)

        # Mostrar contenido del reporte
        self.report_label = QLabel("")
        self.report_label.setWordWrap(True)
        self.report_label.setMinimumHeight(120)
        self.report_label.setStyleSheet("font-size: 12px;")
        right_layout.addWidget(self.report_label)

        right_layout.addStretch()
        main_layout.addLayout(right_layout, 1)

        os.makedirs(self.output_folder, exist_ok=True)
        self.update_output_list()

        self.progress_timer = QTimer(self)
        self.progress_timer.timeout.connect(self.update_progress_from_file)
        self.progress_timer.setInterval(1)  # 1 ms = 0.001 seconds

        self.tester_process = QProcess(self)
        self.tester_process.finished.connect(self.processing_finished)

    def setup_icon_view(self, list_widget):
        list_widget.setViewMode(QListWidget.IconMode)
        list_widget.setIconSize(QSize(100, 100))
        list_widget.setResizeMode(QListWidget.Adjust)
        list_widget.setSpacing(10)
        list_widget.setMovement(QListWidget.Static)

    def select_input_folder(self):
        folder = QFileDialog.getExistingDirectory(self, "Seleccionar carpeta de entrada")
        if folder:
            self.input_folder = folder
            self.input_label.setText(f"Carpeta de entrada: {folder}")
            self.update_input_list()

    def update_input_list(self):
        self.input_list.clear()
        self._load_images_to_list(self.input_folder, self.input_list)

    def update_output_list(self):
        self.output_list.clear()
        self._load_images_to_list(self.output_folder, self.output_list)

    def _load_images_to_list(self, folder, list_widget):
        list_widget.clear()
        valid_extensions = (".bmp",)
        try:
            files = [f for f in os.listdir(folder)
                     if f.lower().endswith(valid_extensions) and os.path.isfile(os.path.join(folder, f))]
            for f in files:
                path = os.path.join(folder, f)
                pixmap = QPixmap(path)
                if not pixmap.isNull():
                    icon = QIcon(pixmap.scaled(100, 100, Qt.KeepAspectRatio, Qt.SmoothTransformation))
                    item = QListWidgetItem(icon, f)
                    list_widget.addItem(item)
                else:
                    list_widget.addItem(QListWidgetItem(f"[ERROR] {f}"))
            if not files:
                list_widget.addItem("No se encontraron imágenes BMP.")
        except Exception as e:
            list_widget.addItem(f"Error al leer la carpeta: {e}")

    def start_processing(self):
        if not self.input_folder:
            self.input_label.setText("⚠️ Debe seleccionar una carpeta de entrada primero.")
            return

        self.write_config()  # Save current settings to config.txt

        self.progress_bar.setValue(0)
        self.process_btn.setEnabled(False)
        self.progress_timer.start()  # Start real-time progress updates

        tester_path = os.path.join(os.path.dirname(__file__), "tester.exe")
        if os.path.exists(tester_path):
            # Ejecuta tester.exe en un hilo para no bloquear la GUI
            threading.Thread(target=self.run_tester_subprocess, args=(tester_path,), daemon=True).start()
        else:
            self.report_label.setText("tester.exe no encontrado.")
            self.progress_timer.stop()
            return

        selected_effects = {
            "grayscale": self.cb_grayscale.isChecked(),
            "flip_h": self.cb_flip_h.isChecked(),
            "flip_v": self.cb_flip_v.isChecked(),
            "gray_flip_h": self.cb_gray_flip_h.isChecked(),
            "gray_flip_v": self.cb_gray_flip_v.isChecked(),
            "blur": self.cb_blur.isChecked(),
            "blur_value": int(self.blur_input.text()) if self.cb_blur.isChecked() else 0
        }

        print(f"Selected effects: {selected_effects}")
        self.report_label.setText("Procesando imágenes...")
        
        self.progress_bar.setValue(0)
        self.process_btn.setEnabled(False)

        self.processing_thread = ImageProcessingThread(self.input_folder, self.output_folder, selected_effects)
        self.processing_thread.progress.connect(self.progress_bar.setValue)
        self.processing_thread.done.connect(self.processing_finished)
        self.processing_thread.start()

    def run_tester_subprocess(self, tester_path):
        try:
            subprocess.run([tester_path], check=True)
        except Exception as e:
            self.report_label.setText(f"Error al ejecutar tester.exe: {e}")
        # Llama a processing_finished en el hilo principal
        QTimer.singleShot(0, self.processing_finished)

    def processing_finished(self):
        self.progress_timer.stop()
        self.process_btn.setEnabled(True)
        self.progress_bar.setValue(100)
        self.update_output_list()
        self.generate_report()
        self.display_report()

    def generate_report(self):
        try:
            files = [f for f in os.listdir(self.input_folder) if f.lower().endswith(".bmp")]
            num_files = len(files)

            effects = []
            if self.cb_grayscale.isChecked(): effects.append("Escala de grises")
            if self.cb_flip_h.isChecked(): effects.append("Volteo horizontal")
            if self.cb_flip_v.isChecked(): effects.append("Volteo vertical")
            if self.cb_gray_flip_h.isChecked(): effects.append("Grises + Volteo horizontal")
            if self.cb_gray_flip_v.isChecked(): effects.append("Grises + Volteo vertical")
            if self.cb_blur.isChecked(): effects.append(f"Desenfoque ({self.blur_input.text()})")

            content = (
                f"REPORTE DE PROCESAMIENTO\n"
                f"-------------------------\n"
                f"Imágenes procesadas: {num_files}\n"
                f"Carpeta de entrada: {self.input_folder}\n"
                f"Carpeta de salida: {os.path.abspath(self.output_folder)}\n"
                f"Efectos aplicados:\n - " + "\n - ".join(effects) + "\n\n"
                f"Fecha y hora: {time.strftime('%Y-%m-%d %H:%M:%S')}\n"
            )

            report_path = os.path.join(self.output_folder, "reporte.txt")
            with open(report_path, "w", encoding="utf-8") as f:
                f.write(content)

        except Exception as e:
            self.report_label.setText(f"Error al generar reporte: {e}")

    def display_report(self):
        report_path = os.path.join(self.output_folder, "reporte.txt")
        if os.path.exists(report_path):
            with open(report_path, "r", encoding="utf-8") as f:
                self.report_label.setText(f.read())
        else:
            self.report_label.setText("No se encontró el archivo de reporte.")

    def write_config(self):
        config_path = os.path.join(os.path.dirname(__file__), "config.txt")
        config_lines = [
            f"grayscale={int(self.cb_grayscale.isChecked())}",
            f"flip_h={int(self.cb_flip_h.isChecked())}",
            f"flip_v={int(self.cb_flip_v.isChecked())}",
            f"gray_flip_h={int(self.cb_gray_flip_h.isChecked())}",
            f"gray_flip_v={int(self.cb_gray_flip_v.isChecked())}",
            f"blur={int(self.cb_blur.isChecked())}",
            f"folder_path={self.input_folder}",
            f"blur_value={self.blur_input.text() if self.cb_blur.isChecked() else '0'}"
        ]
        with open(config_path, "w", encoding="utf-8") as f:
            f.write("\n".join(config_lines))

    def get_total_expected_operations(self):
        # Count .bmp files in input folder
        if not self.input_folder or not os.path.isdir(self.input_folder):
            return 0
        bmp_files = [f for f in os.listdir(self.input_folder) if f.lower().endswith(".bmp")]
        num_files = len(bmp_files)
        # Count checked effect checkboxes
        effects = [
            self.cb_grayscale.isChecked(),
            self.cb_flip_h.isChecked(),
            self.cb_flip_v.isChecked(),
            self.cb_gray_flip_h.isChecked(),
            self.cb_gray_flip_v.isChecked(),
            self.cb_blur.isChecked()
        ]
        num_effects = sum(effects)
        return num_files * num_effects

    def update_progress_from_file(self):
        total = self.get_total_expected_operations()
        if total == 0:
            self.progress_bar.setValue(0)
            return
        try:
            with open("progress.txt", "r") as f:
                count = f.read().count('1')
            percent = int((count / total) * 100)
            self.progress_bar.setValue(min(percent, 100))
        except Exception:
            self.progress_bar.setValue(0)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ImageProcessorGUI()
    window.show()
    sys.exit(app.exec_())
