import subprocess
import os

def run(cmd):

    output=subprocess.check_output(cmd, text=True).strip()
    return output
    
    

def auto_push(remote_url=None):
    # 1. 初始化仓库（如果未初始化）
    if not os.path.exists(".git"):
        run(["git", "init"])

    # 2. 添加文件
    run(["git", "add", "."])

    # 3. 提交
    run(["git", "commit", "-m", "Auto commit"])

    # 4. 检查远程
    try:
        remotes = run(["git", "remote", "-v"])
        if not remotes and remote_url:
            run(["git", "remote", "add", "origin", remote_url])
    except subprocess.CalledProcessError:
        if remote_url:
            run(["git", "remote", "add", "origin", remote_url])

    # 5. 推送
    current_branch = run(["git", "branch", "--show-current"]) or "main"
    run(["git", "push", "-u", "origin", current_branch])

    print("推送完成 ✅")

# 使用示例
#auto_push("git@github.com:username/repo.git")
auto_push("https://github.com/nt47/git-cli.git")