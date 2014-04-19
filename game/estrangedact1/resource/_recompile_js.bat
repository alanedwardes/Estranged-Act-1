@echo off

for %%f in (estrangedact1_*_js.txt) do (
	echo Compiling %%f
	python _kvjsonconverter.py %%f
)

pause