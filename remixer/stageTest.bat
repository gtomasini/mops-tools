::Parametro1: escenario
::recheck this 
::call setEnv.bat
start fusion_app
start assessment_app
start avoidance_app
set stage=%1
if not defined stage (set stage=H1)
::tp server
start tp_serverd %stage%
::input files
set intfile=in/%stage%_Truth_TVInt1.csv
set ownfile=in/%stage%_Truth_TVOwn.csv
set callsign=%stage%_INTR
::batidora starts
echo TESTING %stage% STAGE....
call remixerd /truthcsv=%intfile% /ownshipcsv=%ownfile% /callsign=%callsign%
::kill daa apps
taskkill /IM fusion_app.exe
taskkill /IM assessment_app.exe
taskkill /IM avoidance_app.exe
::just in case
taskkill /IM tp_serverd.exe
echo FIN %stage% 