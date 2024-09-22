import sublime
import sublime_plugin
import re
from pathlib import Path
from typing import List

# 设置库路径
lib_path = Path("/Users/noya/libra")

# 正则表达式用于匹配 #include 指令和 include guard
atcoder_include = re.compile('#include\s*["<](atcoder/[a-z_]*(|.hpp))[">]\s*')
include_guard = re.compile('#.*ATCODER_[A-Z_]*_HPP')

noya_include = re.compile('#include\s*["<](noya/[a-z_]*(|.hpp))[">]\s*')
noya_guard = re.compile('#.*NOYA_[A-Z_]*_HPP')



# 递归查找和扩展 #include 指令
defined = set()

def dfs(f: str) -> List[str]:
    global defined
    if f in defined:
        return []

    result = []
    defined.add(f)
    s = open(str(lib_path / f)).read()
    for line in s.splitlines():
        if include_guard.match(line):
            continue
        if noya_guard.match(line):
            continue
        m = atcoder_include.match(line)
        if m:
            result.extend(dfs(m.group(1)))
            continue
        m = noya_include.match(line)
        if m:
            result.extend(dfs(m.group(1)))
            continue
        result.append(line)
    return result

# 自定义复制命令，扩展剪贴板内容
class CopyMagicCommand(sublime_plugin.TextCommand):
    def run(self, edit):
        # 获取当前选择的文本
        selected_text = self.view.substr(self.view.sel()[0])
        # 扩展剪贴板内容
        expanded_text = self.expand_clipboard(selected_text)
        # 将扩展后的内容设置到剪贴板
        sublime.set_clipboard(expanded_text)

    def is_enabled(self):
        return True

    def expand_clipboard(self, clipboard_content):
        defined.clear()
        result = []
        for line in clipboard_content.splitlines():
            m = atcoder_include.match(line)
            if m:
                result.extend(dfs(m.group(1)))
                continue
            m = noya_include.match(line)
            if m:
                result.extend(dfs(m.group(1)))
                continue
            result.append(line)
        return '\r\n'.join(result) + '\r\n'

# 每次复制操作时触发扩展剪贴板内容的命令
class EventListener(sublime_plugin.EventListener):
    def on_text_command(self, view, command_name, args):
        if command_name == "copy":
            view.run_command("copy_magic")
            return ("noop")
