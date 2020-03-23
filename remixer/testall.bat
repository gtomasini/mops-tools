echo TEST ALL STAGES
::kill daa apps
taskkill /IM fusion_app.exe
taskkill /IM assessment_app.exe
taskkill /IM avoidance_app.exe
::just in case
taskkill /IM tp_serverd.exe
call setenv.bat
call stagetest H1
call stagetest H2
call stagetest H3
call stagetest H4
call stagetest H5
call stagetest H6
call stagetest H7
call stagetest H8
call stagetest H9
call stagetest H10
