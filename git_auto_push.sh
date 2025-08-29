#!/bin/bash

# 检查是否提供了提交信息
if [ -z "$1" ]; then
  echo "❌ 错误：请在命令后添加提交信息"
  echo "✅ 示例用法：./git_auto_push.sh \"修复了布局问题\""
  exit 1
fi

commit_msg="$1"

# 检查是否已有相同提交信息
if git log --pretty=format:"%s" | grep -Fxq "$commit_msg"; then
  echo "⚠️ 提交信息 \"$commit_msg\" 已经存在于历史记录中。"
  read -p "是否仍然继续提交？[y/N]: " confirm
  confirm=${confirm:-n}

  if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo "🚫 取消提交。"
    exit 0
  fi
fi

# 执行 Git 操作
git add .
git commit -m "$commit_msg"
git push
