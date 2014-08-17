# TextEngine 语法规则
###需要修改的文件
		File <文件名> // 请置于首行
###转到行号并修改内容
		Line <行号>
	或者
		Line <行号>-<行号>
###新建行
	New <行号>
	或者
	New <行号>-<行号>

###使用以上两个命令需要匹配与其数量相符的Text命令，如
		Line 1-3
		Text Line1
		Text Line2
		Text Line3

###清空
		Empty <行号>
###删除
		Delete <行号>
###替换
		Replace <行号> <需替换的内容> <被替换的内容>