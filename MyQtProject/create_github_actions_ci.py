#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GitHub Actions CI/CD 配置文件生成工具
使用 PyQt 创建可视化界面,生成跨平台 CI/CD 配置文件
"""

import sys
import os
from pathlib import Path
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QLabel, QLineEdit, QPushButton, 
                             QTextEdit, QCheckBox, QComboBox, QGroupBox,
                             QFileDialog, QMessageBox, QSpinBox, QGridLayout,
                             QScrollArea)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QFont


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("GitHub Actions CI/CD 配置生成工具")
        self.setGeometry(100, 100, 900, 800)
        
        # 创建中央部件和滚动区域
        scroll_area = QScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)
        
        # 创建滚动内容部件
        central_widget = QWidget()
        scroll_area.setWidget(central_widget)
        self.setCentralWidget(scroll_area)
        
        # 主布局
        main_layout = QVBoxLayout(central_widget)
        main_layout.setSpacing(15)
        main_layout.setContentsMargins(15, 15, 15, 15)
        
        # 标题
        title_label = QLabel("GitHub Actions CI/CD 配置生成工具")
        title_font = QFont()
        title_font.setPointSize(16)
        title_font.setBold(True)
        title_label.setFont(title_font)
        title_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        main_layout.addWidget(title_label)
        
        # 项目信息组
        project_group = QGroupBox("项目信息")
        project_layout = QGridLayout()
        
        project_layout.addWidget(QLabel("项目名称:"), 0, 0)
        self.project_name_edit = QLineEdit()
        self.project_name_edit.setPlaceholderText("例如: my_qt_project")
        project_layout.addWidget(self.project_name_edit, 0, 1)
        
        project_layout.addWidget(QLabel("Qt 版本:"), 1, 0)
        self.qt_version_combo = QComboBox()
        self.qt_version_combo.addItems(["6.8.3", "6.8.0", "6.7.0", "6.6.0", "6.5.0", "6.4.0", "6.0.0", "5.15.2"])
        self.qt_version_combo.setCurrentText("6.8.3")
        project_layout.addWidget(self.qt_version_combo, 1, 1)
        
        project_layout.addWidget(QLabel("Python 版本:"), 2, 0)
        self.python_version_edit = QLineEdit()
        self.python_version_edit.setText("3.10")
        project_layout.addWidget(self.python_version_edit, 2, 1)
        
        project_layout.addWidget(QLabel("构建类型:"), 3, 0)
        self.build_type_combo = QComboBox()
        self.build_type_combo.addItems(["Debug", "Release"])
        project_layout.addWidget(self.build_type_combo, 3, 1)
        
        project_group.setLayout(project_layout)
        main_layout.addWidget(project_group)
        
        # 平台选择组
        platform_group = QGroupBox("支持平台 (多选)")
        platform_layout = QVBoxLayout()
        
        self.windows_checkbox = QCheckBox("Windows (windows-latest)")
        self.windows_checkbox.setChecked(True)
        platform_layout.addWidget(self.windows_checkbox)
        
        self.macos_checkbox = QCheckBox("macOS (macos-latest)")
        self.macos_checkbox.setChecked(True)
        platform_layout.addWidget(self.macos_checkbox)
        
        self.ubuntu_checkbox = QCheckBox("Ubuntu (ubuntu-latest)")
        self.ubuntu_checkbox.setChecked(True)
        platform_layout.addWidget(self.ubuntu_checkbox)
        
        platform_group.setLayout(platform_layout)
        main_layout.addWidget(platform_group)
        
        # 触发条件组
        trigger_group = QGroupBox("CI/CD 触发条件")
        trigger_layout = QVBoxLayout()
        
        self.push_checkbox = QCheckBox("推送代码时触发 (push)")
        self.push_checkbox.setChecked(True)
        trigger_layout.addWidget(self.push_checkbox)
        
        self.pull_request_checkbox = QCheckBox("Pull Request 时触发 (pull_request)")
        self.pull_request_checkbox.setChecked(True)
        trigger_layout.addWidget(self.pull_request_checkbox)
        
        self.tag_checkbox = QCheckBox("发布标签时触发 (tags: 'v*')")
        self.tag_checkbox.setChecked(False)
        trigger_layout.addWidget(self.tag_checkbox)
        
        self.workflow_dispatch_checkbox = QCheckBox("手动触发 (workflow_dispatch)")
        self.workflow_dispatch_checkbox.setChecked(False)
        trigger_layout.addWidget(self.workflow_dispatch_checkbox)
        
        trigger_group.setLayout(trigger_layout)
        main_layout.addWidget(trigger_group)
        
        # 额外选项组
        options_group = QGroupBox("额外选项")
        options_layout = QVBoxLayout()
        
        self.upload_artifact_checkbox = QCheckBox("上传构建产物 (Artifacts)")
        self.upload_artifact_checkbox.setChecked(True)
        options_layout.addWidget(self.upload_artifact_checkbox)
        
        self.cache_qt_checkbox = QCheckBox("启用 Qt 缓存 (加速构建)")
        self.cache_qt_checkbox.setChecked(True)
        options_layout.addWidget(self.cache_qt_checkbox)
        
        options_group.setLayout(options_layout)
        main_layout.addWidget(options_group)
        
        # 输出目录选择
        output_group = QGroupBox("输出设置")
        output_layout = QHBoxLayout()
        
        output_layout.addWidget(QLabel("输出目录:"))
        self.output_path_edit = QLineEdit()
        self.output_path_edit.setText(".github/workflows")
        output_layout.addWidget(self.output_path_edit)
        
        browse_button = QPushButton("浏览...")
        browse_button.clicked.connect(self.browse_output_path)
        output_layout.addWidget(browse_button)
        
        output_group.setLayout(output_layout)
        main_layout.addWidget(output_group)
        
        # 按钮区域
        button_layout = QHBoxLayout()
        button_layout.addStretch()
        
        self.generate_button = QPushButton("生成配置文件")
        self.generate_button.setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px 20px; font-weight: bold; }")
        self.generate_button.clicked.connect(self.generate_config)
        button_layout.addWidget(self.generate_button)
        
        self.preview_button = QPushButton("预览配置")
        self.preview_button.clicked.connect(self.preview_config)
        button_layout.addWidget(self.preview_button)
        
        button_layout.addStretch()
        main_layout.addLayout(button_layout)
        
        # 预览文本框
        preview_label = QLabel("配置文件预览:")
        main_layout.addWidget(preview_label)
        
        self.preview_text = QTextEdit()
        self.preview_text.setReadOnly(True)
        self.preview_text.setFont(QFont("Consolas", 10))
        self.preview_text.setMinimumHeight(200)
        main_layout.addWidget(self.preview_text)
        
        # 添加底部间距
        main_layout.addStretch()
    
    def browse_output_path(self):
        directory = QFileDialog.getExistingDirectory(self, "选择输出目录", self.output_path_edit.text())
        if directory:
            self.output_path_edit.setText(directory)
    
    def get_selected_platforms(self):
        """获取选中的平台列表"""
        platforms = []
        if self.windows_checkbox.isChecked():
            platforms.append(("windows-latest", "windows"))
        if self.macos_checkbox.isChecked():
            platforms.append(("macos-latest", "mac"))
        if self.ubuntu_checkbox.isChecked():
            platforms.append(("ubuntu-latest", "linux"))
        return platforms
    
    def get_trigger_conditions(self):
        """获取触发条件"""
        conditions = {}
        if self.push_checkbox.isChecked():
            conditions["push"] = True
        if self.pull_request_checkbox.isChecked():
            conditions["pull_request"] = True
        if self.tag_checkbox.isChecked():
            conditions["tags"] = "v*"
        if self.workflow_dispatch_checkbox.isChecked():
            conditions["workflow_dispatch"] = True
        return conditions
    
    def generate_github_actions_config(self):
        """生成 GitHub Actions 配置文件内容"""
        project_name = self.project_name_edit.text() or "my_qt_project"
        qt_version = self.qt_version_combo.currentText()
        python_version = self.python_version_edit.text() or "3.10"
        build_type = self.build_type_combo.currentText()
        platforms = self.get_selected_platforms()
        triggers = self.get_trigger_conditions()
        
        if not platforms:
            return None, "请至少选择一个平台!"
        
        # 构建 on 部分
        on_lines = ["on:"]
        has_push = False
        
        if triggers.get("push") or triggers.get("tags"):
            on_lines.append("  push:")
            has_push = True
            if triggers.get("tags"):
                on_lines.append("    tags:")
                on_lines.append(f"      - '{triggers['tags']}'")
            elif not triggers.get("push"):  # 如果只有 tags 触发，添加默认的 v* 标签
                on_lines.append("    tags:")
                on_lines.append("      - 'v*'")
        
        if triggers.get("pull_request"):
            on_lines.append("  pull_request:")
            on_lines.append("    branches: [ main, master ]")
        
        if triggers.get("workflow_dispatch"):
            on_lines.append("  workflow_dispatch:")
        
        # 如果没有选择任何触发条件,默认使用 push
        if not triggers:
            on_lines.append("  push:")
        
        # 构建 jobs 部分 - 使用 include 格式明确指定架构
        matrix_includes = []
        arch_map = {
            ("windows-latest", "windows"): "win64_msvc2022_64",
            ("macos-latest", "mac"): "clang_64",
            ("ubuntu-latest", "linux"): "gcc_64"
        }
        
        for os_name, host in platforms:
            arch = arch_map.get((os_name, host), "gcc_64")
            matrix_includes.append(f"        - os: {os_name}")
            matrix_includes.append(f"          host: {host}")
            matrix_includes.append(f"          arch: {arch}")
        
        config = f"""name: CI / Build / Package

{chr(10).join(on_lines)}

jobs:
  build:
    name: Build on ${{{{ matrix.os }}}}
    runs-on: ${{{{ matrix.os }}}}
    strategy:
      fail-fast: false
      matrix:
        include:
{chr(10).join(matrix_includes)}

    steps:
    - name: Checkout code
      uses: actions/checkout@v4

    - name: Setup Python
      uses: actions/setup-python@v4
      with:
        python-version: '{python_version}'
"""
        
        # 根据选择的平台生成对应的 Install Qt 步骤
        install_qt_steps = []
        platform_qt_configs = {
            "windows": {
                "name": "Install Qt (Windows)",
                "host": "windows",
                "arch": "win64_msvc2022_64"
            },
            "mac": {
                "name": "Install Qt (macOS)",
                "host": "mac",
                "arch": "clang_64"
            },
            "linux": {
                "name": "Install Qt (Linux)",
                "host": "linux",
                "arch": None  # Linux 不指定 arch，让 action 自动检测
            }
        }
        
        for os_name, host in platforms:
            qt_config = platform_qt_configs.get(host)
            if qt_config:
                step = f"""    - name: {qt_config['name']}
      uses: jurplel/install-qt-action@v4
      if: matrix.host == '{host}'
      with:
        version: '{qt_version}'
        host: {qt_config['host']}
        target: desktop"""
                if qt_config['arch']:
                    step += f"""
        arch: {qt_config['arch']}"""
                step += f"""
        cache: {'true' if self.cache_qt_checkbox.isChecked() else 'false'}"""
                install_qt_steps.append(step)
        
        config += "\n".join(install_qt_steps) + f"""

    - name: Setup CMake
      uses: jwlawson/actions-setup-cmake@v1.14
      with:
        cmake-version: '3.27'

    - name: Configure CMake
      run: |
        cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

    - name: Build
      run: |
        cmake --build build --config Release

    - name: Find Executable
      id: find_exe
      shell: bash
      run: |
        if [ "${{{{ matrix.host }}}}" == "windows" ]; then
          echo "Searching for Windows executable..."
          echo "Build directory contents:"
          ls -la build/ 2>/dev/null || echo "build/ not found"
          ls -la build/Release/ 2>/dev/null || echo "build/Release/ not found"
          ls -la build/bin/ 2>/dev/null || echo "build/bin/ not found"
          
          found_exe=""
          if command -v find &> /dev/null; then
            found_exe=$(find build -name "{project_name}.exe" -type f 2>/dev/null | head -1)
          fi
          
          if [ -n "$found_exe" ]; then
            echo "Found executable via find: $found_exe"
            echo "exe_path=$found_exe" >> $GITHUB_OUTPUT
            echo "exe_dir=$(dirname $found_exe)" >> $GITHUB_OUTPUT
          else
            echo "Checking common locations..."
            if [ -f "build/Release/{project_name}.exe" ]; then
              echo "Found at: build/Release/{project_name}.exe"
              echo "exe_path=build/Release/{project_name}.exe" >> $GITHUB_OUTPUT
              echo "exe_dir=build/Release" >> $GITHUB_OUTPUT
            elif [ -f "build/bin/Release/{project_name}.exe" ]; then
              echo "Found at: build/bin/Release/{project_name}.exe"
              echo "exe_path=build/bin/Release/{project_name}.exe" >> $GITHUB_OUTPUT
              echo "exe_dir=build/bin/Release" >> $GITHUB_OUTPUT
            elif [ -f "build/bin/{project_name}.exe" ]; then
              echo "Found at: build/bin/{project_name}.exe"
              echo "exe_path=build/bin/{project_name}.exe" >> $GITHUB_OUTPUT
              echo "exe_dir=build/bin" >> $GITHUB_OUTPUT
            else
              echo "ERROR: Executable not found in any expected location"
            fi
          fi
        elif [ "${{{{ matrix.host }}}}" == "mac" ]; then
          if [ -d "build/bin/{project_name}.app" ]; then
            echo "exe_path=build/bin/{project_name}.app" >> $GITHUB_OUTPUT
            echo "exe_dir=build/bin" >> $GITHUB_OUTPUT
          elif [ -f "build/bin/{project_name}" ]; then
            echo "exe_path=build/bin/{project_name}" >> $GITHUB_OUTPUT
            echo "exe_dir=build/bin" >> $GITHUB_OUTPUT
          fi
        else
          if [ -f "build/bin/{project_name}" ]; then
            echo "exe_path=build/bin/{project_name}" >> $GITHUB_OUTPUT
            echo "exe_dir=build/bin" >> $GITHUB_OUTPUT
          fi
        fi

    - name: Package (Windows)
      if: matrix.host == 'windows'
      shell: pwsh
      run: |
        Write-Host "Debug: Searching for executable..."
        Write-Host "exe_path output: ${{{{ steps.find_exe.outputs.exe_path }}}}"
        Write-Host "exe_dir output: ${{{{ steps.find_exe.outputs.exe_dir }}}}"
        
        $possiblePaths = @(
          "build\\Release\\{project_name}.exe",
          "build\\bin\\Release\\{project_name}.exe",
          "build\\bin\\{project_name}.exe",
          "build\\{project_name}.exe"
        )
        
        $exePath = ""
        $exeDir = ""
        
        foreach ($path in $possiblePaths) {{
          if (Test-Path $path) {{
            $exePath = $path
            $exeDir = Split-Path -Parent $path
            Write-Host "Found executable at: $exePath"
            break
          }}
        }}
        
        if (-not $exePath) {{
          $exePath = "${{{{ steps.find_exe.outputs.exe_path }}}}"
          $exeDir = "${{{{ steps.find_exe.outputs.exe_dir }}}}"
        }}
        
        if ($exePath -and (Test-Path $exePath)) {{
          windeployqt --release --compiler-runtime $exePath
          $packageDir = "package"
          New-Item -ItemType Directory -Force -Path $packageDir | Out-Null
          Copy-Item -Path $exeDir\* -Destination $packageDir -Recurse -Force
          $runScriptContent = "@echo off`r`ncd /d `"%~dp0`"`r`nstart `"`" `"{project_name}.exe`""
          [System.IO.File]::WriteAllText("$packageDir\运行程序.bat", $runScriptContent, [System.Text.Encoding]::UTF8)
          
          Write-Host "Package created in: $packageDir"
        }} else {{
          Write-Host "Error: Executable not found"
          exit 1
        }}

    - name: Package (macOS)
      if: matrix.host == 'mac'
      shell: bash
      run: |
        exe_path="${{{{ steps.find_exe.outputs.exe_path }}}}"
        exe_dir="${{{{ steps.find_exe.outputs.exe_dir }}}}"
        if [ -n "$exe_path" ] && [ -e "$exe_path" ]; then
          if [[ "$exe_path" == *.app ]]; then
            macdeployqt "$exe_path" -always-overwrite
            package_dir="package"
            mkdir -p "$package_dir"
            cp -R "$exe_path" "$package_dir/"
          else
            app_name="{project_name}.app"
            app_path="package/$app_name"
            mkdir -p "$app_path/Contents/MacOS"
            mkdir -p "$app_path/Contents/Frameworks"
            mkdir -p "$app_path/Contents/Resources"
            
            cp "$exe_path" "$app_path/Contents/MacOS/{project_name}"
            chmod +x "$app_path/Contents/MacOS/{project_name}"
            printf '<?xml version="1.0" encoding="UTF-8"?>\\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\\n<plist version="1.0">\\n<dict>\\n  <key>CFBundleExecutable</key>\\n  <string>{project_name}</string>\\n  <key>CFBundleIdentifier</key>\\n  <string>com.example.{project_name}</string>\\n  <key>CFBundleName</key>\\n  <string>{project_name}</string>\\n  <key>CFBundlePackageType</key>\\n  <string>APPL</string>\\n  <key>CFBundleVersion</key>\\n  <string>1.0</string>\\n</dict>\\n</plist>\\n' > "$app_path/Contents/Info.plist"
            macdeployqt "$app_path" -always-overwrite
          fi
          printf '#!/bin/bash\\ncd "$(dirname "$0")"\\nopen "{project_name}.app"\\n' > "package/运行程序.sh"
          chmod +x "package/运行程序.sh"
          
          echo "Package created in: package/"
        else
          echo "Error: Executable not found"
          exit 1
        fi

    - name: Install Linux Deployment Tools
      if: matrix.host == 'linux'
      run: |
        sudo apt-get update
        sudo apt-get install -y patchelf

    - name: Package (Linux)
      if: matrix.host == 'linux'
      shell: bash
      run: |
        exe_path="${{{{ steps.find_exe.outputs.exe_path }}}}"
        exe_dir="${{{{ steps.find_exe.outputs.exe_dir }}}}"
        if [ -n "$exe_path" ] && [ -e "$exe_path" ]; then
          echo "Packaging Linux application: $exe_path"
          
          package_dir="package"
          mkdir -p "$package_dir"
          cp "$exe_path" "$package_dir/{project_name}"
          chmod +x "$package_dir/{project_name}"
          qt_prefix="${{{{ env.Qt6_DIR }}}}"
          if [ -z "$qt_prefix" ] || [ ! -d "$qt_prefix" ]; then
            if command -v qmake6 &> /dev/null; then
              qt_prefix=$(qmake6 -query QT_INSTALL_PREFIX)
            elif command -v qmake &> /dev/null; then
              qt_prefix=$(qmake -query QT_INSTALL_PREFIX)
            fi
          fi
          
          if [ -n "$qt_prefix" ] && [ -d "$qt_prefix" ]; then
            echo "Qt prefix found: $qt_prefix"
            
            lib_dir="$package_dir/lib"
            plugins_dir="$package_dir/plugins"
            mkdir -p "$lib_dir" "$plugins_dir"
            ldd "$package_dir/{project_name}" | grep -i qt | while read line; do
              lib_file=$(echo "$line" | awk '{{print $3}}')
              if [ -n "$lib_file" ] && [ -f "$lib_file" ]; then
                cp "$lib_file" "$lib_dir/"
                if [ -L "$lib_file" ]; then
                  real_file=$(readlink -f "$lib_file")
                  if [ -f "$real_file" ]; then
                    cp "$real_file" "$lib_dir/"
                  fi
                fi
              fi
            done
            qt_plugins="$qt_prefix/plugins"
            if [ -d "$qt_plugins/platforms" ]; then
              cp -r "$qt_plugins/platforms" "$plugins_dir/"
            fi
            for plugin_dir in imageformats iconengines platformthemes; do
              if [ -d "$qt_plugins/$plugin_dir" ]; then
                cp -r "$qt_plugins/$plugin_dir" "$plugins_dir/" 2>/dev/null || true
              fi
            done
            if command -v patchelf &> /dev/null; then
              patchelf --set-rpath '$ORIGIN/lib' "$package_dir/{project_name}" 2>/dev/null || true
            fi
            printf '#!/bin/bash\\nSCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"\\ncd "$SCRIPT_DIR"\\nexport LD_LIBRARY_PATH="$SCRIPT_DIR/lib:$LD_LIBRARY_PATH"\\nexport QT_PLUGIN_PATH="$SCRIPT_DIR/plugins"\\n./{project_name} "$@"\\n' > "$package_dir/运行程序.sh"
            chmod +x "$package_dir/运行程序.sh"
          else
            printf '#!/bin/bash\\nSCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"\\ncd "$SCRIPT_DIR"\\n./{project_name} "$@"\\n' > "$package_dir/运行程序.sh"
            chmod +x "$package_dir/运行程序.sh"
          fi
          
          echo "Package created in: $package_dir/"
        else
          echo "Error: Executable not found"
          exit 1
        fi
"""
        
        if self.upload_artifact_checkbox.isChecked():
            config += f"""
    - name: Upload Artifact
      uses: actions/upload-artifact@v4
      with:
        name: {project_name}-${{{{ matrix.os }}}}-${{{{ github.run_number }}}}
        path: |
          package/**
        if-no-files-found: warn
        retention-days: 30

  release:
    name: Create Release
    needs: build
    if: startsWith(github.ref, 'refs/tags/v')
    runs-on: ubuntu-latest
    permissions:
      contents: write
    
    steps:
    - name: Download all artifacts
      uses: actions/download-artifact@v4
      with:
        path: artifacts
    
    - name: Extract version from tag
      id: tag_version
      run: |
        TAG_NAME=${{{{{{ GITHUB_REF#refs/tags/ }}}}}}
        echo "tag_name=$TAG_NAME" >> $GITHUB_OUTPUT
        VERSION=${{{{{{ TAG_NAME#v }}}}}}
        echo "version=$VERSION" >> $GITHUB_OUTPUT
    
    - name: Create Release
      uses: softprops/action-gh-release@v1
      with:
        tag_name: ${{{{ steps.tag_version.outputs.tag_name }}}}
        name: Release ${{{{ steps.tag_version.outputs.version }}}}
        body: |
          ## Release ${{{{ steps.tag_version.outputs.version }}}}
          
          ### 下载
          
          - **Windows**: 下载 `{project_name}-windows-latest-*.zip`，解压后运行 `运行程序.bat` 或直接运行 `{project_name}.exe`
          - **macOS**: 下载 `{project_name}-macos-latest-*.zip`，解压后运行 `运行程序.sh` 或双击 `{project_name}.app`
          - **Linux**: 下载 `{project_name}-ubuntu-latest-*.zip`，解压后运行 `./运行程序.sh` 或直接运行 `./{project_name}`
          
          ### 系统要求
          
          - Windows: Windows 10 或更高版本
          - macOS: macOS 10.13 或更高版本
          - Linux: Ubuntu 20.04+ / Debian 11+ / Fedora 34+ 等
          
          **注意**: 所有依赖已包含在包中，无需安装 Qt 或其他依赖。
        files: |
          artifacts/**/*
        draft: false
        prerelease: false
"""
        
        return config, None
    
    def preview_config(self):
        """预览配置文件"""
        config, error = self.generate_github_actions_config()
        if error:
            QMessageBox.warning(self, "错误", error)
            return
        self.preview_text.setPlainText(config)
    
    def generate_config(self):
        """生成并保存配置文件"""
        project_name = self.project_name_edit.text() or "my_qt_project"
        
        if not project_name:
            QMessageBox.warning(self, "错误", "请输入项目名称!")
            return
        
        config, error = self.generate_github_actions_config()
        if error:
            QMessageBox.warning(self, "错误", error)
            return
        
        # 预览配置
        self.preview_text.setPlainText(config)
        
        # 确认保存
        reply = QMessageBox.question(
            self, 
            "确认", 
            f"将保存配置文件到:\n{self.output_path_edit.text()}/ci.yml\n\n是否继续?",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )
        
        if reply == QMessageBox.StandardButton.Yes:
            try:
                output_dir = Path(self.output_path_edit.text())
                output_dir.mkdir(parents=True, exist_ok=True)
                
                config_file = output_dir / "ci.yml"
                config_file.write_text(config, encoding='utf-8')
                
                QMessageBox.information(
                    self, 
                    "成功", 
                    f"配置文件已成功生成!\n\n位置: {config_file.absolute()}\n\n"
                    f"请确保将生成的 .github/workflows/ 目录提交到 Git 仓库。"
                )
            except Exception as e:
                QMessageBox.critical(self, "错误", f"保存文件时出错:\n{str(e)}")


def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')  # 使用 Fusion 样式,更好的跨平台体验
    
    window = MainWindow()
    window.show()
    
    sys.exit(app.exec())


if __name__ == "__main__":
    main()