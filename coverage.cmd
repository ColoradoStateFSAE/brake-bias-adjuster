@echo off

for /r %%G in (*.gcda) do del "%%G"

pio test -v -e test_mingw

IF %ERRORLEVEL% EQU 0 (
    IF EXIST "coverage" (
        rmdir /s /q "coverage"
    )

    mkdir "coverage"

    gcovr -r . --html-details -o ./coverage/coverage_report.html --exclude "(\.pio|test)" --exclude-unreachable-branches
)
