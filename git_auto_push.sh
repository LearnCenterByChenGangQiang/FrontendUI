#!/bin/bash

# 初始化变量
commit_msg=""
do_push=false
show_help=false

# 显示帮助信息
show_usage() {
  echo "📝 Git 自动提交脚本（默认仅 commit，加 -push 才推送）"
  echo "=============================================="
  echo "用法：./git_auto_push.sh [提交信息] [-push] [-h/--help]"
  echo ""
  echo "参数说明："
  echo "  提交信息        （必填）本次提交的描述，需用引号包裹（支持空格）"
  echo "  -push           （可选）执行 commit 后自动推送至远程仓库"
  echo "  -h / --help     （可选）显示本帮助信息"
  echo ""
  echo "使用示例："
  echo "  1. 仅执行 commit（不推送）："
  echo "     ./git_auto_push.sh \"修复首页布局错乱问题\""
  echo ""
  echo "  2. 执行 commit + push（提交并推送）："
  echo "     ./git_auto_push.sh \"优化接口响应速度\" -push"
  echo ""
  echo "  3. 显示帮助信息："
  echo "     ./git_auto_push.sh -h"
  echo "     ./git_auto_push.sh --help"
  echo "=============================================="
}

# 解析命令行参数
for arg in "$@"; do
  case "$arg" in
    -h|--help)
      show_help=true
      ;;
    -push)
      do_push=true
      ;;
    *)
      # 提交信息（如果已有提交信息，忽略后续非参数内容）
      if [ -z "$commit_msg" ]; then
        commit_msg="$arg"
      fi
      ;;
  esac
done

# 优先处理帮助信息
if $show_help; then
  show_usage
  exit 0
fi

# 检查是否提供了提交信息（非帮助模式下）
if [ -z "$commit_msg" ]; then
  echo "❌ 错误：请提供提交信息（使用 -h/--help 查看用法）"
  echo "✅ 示例：./git_auto_push.sh \"修复了布局问题\""
  exit 1
fi

# 检查是否已有相同提交信息
if git log --pretty=format:"%s" | grep -Fxq "$commit_msg"; then
  echo "⚠️ 提交信息 \"$commit_msg\" 已经存在于历史记录中。"
  read -p "是否仍然继续？[y/N]: " confirm
  confirm=${confirm:-n}

  if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo "🚫 操作取消。"
    exit 0
  fi
fi

# 执行 Git 操作
echo "📥 开始执行 commit 操作..."
git add .
git commit -m "$commit_msg"
echo "✅ commit 完成！"

# 如果添加了 -push 参数，执行推送
if $do_push; then
  echo "📤 开始执行 push 操作..."
  git push
  echo "✅ push 完成！"
else
  echo "💡 已仅执行 commit（未推送），如需推送请添加 -push 参数"
  echo "   示例：./git_auto_push.sh \"$commit_msg\" -push"
fi