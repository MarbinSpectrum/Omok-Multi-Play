:: cocos new [������Ʈ �̸�] -p [��Ű����] -l [������ ���] -d [������Ʈ ������]

@echo off

set /p projectName="������Ʈ �̸� : "
set /p packageName="��Ű�� �� : "

cocos new %projectName% -p %packageName% -l cpp -d .