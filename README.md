# DynamicMacro
This program allows you to record and use macros on demand.

If you execute the program with the 'CTR' key pressed, it will enter Read mode, otherwise it will enter Write mode.

# Read mode: 
  The program will record all keyboard inputs until 'Esc' key pressed. Then will store the result as a command to be reprodeced later.

# Write mode:
  The program will search for the recored command and execute it.

# Setup
Open the task scheduler (Win+R -> taskschd.msc) and create a new task called "DynamicMacro".
On 'General', ensure the checkboxes "Execute only when user logged in" and "Execute with high privileges".
On 'Action' select "open program" and execute the "Init.bat" file inside ThisProgram/Config.

# Usage
Launch the "Command1.bat" to execute the program (no run as admin is needed since the scheduler already has the permissions).
You can duplicate this bat file and change the command's name to add as many commands as you want.
