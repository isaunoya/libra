import os

def generate_include_file(directory, output_file):
    with open(output_file, 'w') as outfile:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".hpp") and file != "all.hpp":
                    relative_path = os.path.relpath(os.path.join(root, file), directory)
                    include_statement = f'#include "{relative_path}"\n'
                    outfile.write(include_statement)
    print(f"Generated {output_file} with all .hpp includes.")

# 使用示例
directory = "."  # 替换为你的文件夹路径
output_file = "noya/all.hpp"       # 输出文件名
generate_include_file(directory, output_file)