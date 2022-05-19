:: cocos new [프로젝트 이름] -p [패키지명] -l [개발할 언어] -d [프로젝트 만들경로]

@echo off

set /p projectName="프로젝트 이름 : "
set /p packageName="패키지 명 : "

cocos new %projectName% -p %packageName% -l cpp -d .