cmd_/home/pi/LDD-010/Ques-1/Q-2/modules.order := {   echo /home/pi/LDD-010/Ques-1/Q-2/main.ko; :; } | awk '!x[$$0]++' - > /home/pi/LDD-010/Ques-1/Q-2/modules.order
