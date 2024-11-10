import os
import subprocess

# 编译器和选项
COMPILER = "g++"
CXX_STANDARD = "-std=c++23"
OPTIMIZATION = "-O2"
WARNINGS = "-Wall"
DEFINE = "-Dlocal"
INCLUDE_DIR = "-IC:/Users/lenovo/Desktop/libra"  # 去掉了引号转义

# 目录设置
SEARCH_DIR = "./"

def generate_gch_files(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith("head.hpp") or file.endswith("all.hpp"):
                header_file = os.path.join(root, file)
                gch_file = f"{header_file}.gch"
                
                # 打印正在生成的信息
                print(f"Generating precompiled header for {header_file}")
                
                # 生成 .gch 文件
                result = subprocess.run([
                    COMPILER,
                    CXX_STANDARD,
                    OPTIMIZATION,
                    WARNINGS,
                    DEFINE,
                    INCLUDE_DIR,
                    "-x", "c++-header",
                    header_file,
                    "-o", gch_file
                ], capture_output=True, text=True)
                
                # 检查生成是否成功
                if result.returncode == 0:
                    print(f"Successfully generated {gch_file}")
                else:
                    print(f"Failed to generate {gch_file}")
                    print(result.stderr)

if __name__ == "__main__":
    generate_gch_files(SEARCH_DIR)
