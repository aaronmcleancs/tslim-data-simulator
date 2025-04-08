COMP3004 Final Project
======================
Video
-----
- Demonstration video
  https://www.youtube.com/watch?v=QI7yhzhIrkM
- Scenario Testing video
  https://www.youtube.com/watch?v=499WOtazRU8

USAGE
-----
Tested for compilation on the COMP3004 VM*
    ~qmake
    ~make
    ~./COMP3004A4

TEAM MEMBERS
-------------------------------
- Shreyansh Patel
- Alan Mirza
- Mohammad Abbas
- Félix Aylen
- Aaron McLean

TEAM MEMBERS & DETAILED RESPONSIBILITIES
------------------------------------------
**Alan Mirza**
- **UI Design & Navigation:**  
  Developed the initial UI skeleton and designed the home screen. 
- **Implemented battery draining/charging. 
- **Implemented power off/on fucntionality.
- **Security Implementation:**  
  Integrated PIN security measures to add safety to user details.
- **Component Integration & Testing:**  
  Testing of UI components, coherent interface functionality.
- **Documentation & Traceability:**  
  Contributed to the traceability matrix and project documentation.

**Shreyansh Patel**
- **Use Case Documentation & Bolus Calculator Design:**  
  Authored detailed documentation and designed the bolus calculation feature.
- **Manual Bolus Functionality:**  
  Implemented manual bolus operations.
- **Extended Bolus Implementation:**  
  Enhanced bolus features for extended use scenarios.
- **Testing & Demonstration:**  
  Conducted scenario tests and prepared demonstration materials to validate bolus calculator.

**Félix Aylen**
- **System Architecture & Profile Management:**  
  Designed the class diagram and developed the structure for managing profiles.
- **User Profiles (CRUD Operations):**  
  Implemented complete profile management including creation, editing, and deletion.
- **Data Visualization:**  
  Developed data visualization components to display the glucose information.
- **UI Component Integration:**  
  Integration of visualization elements with the user interface.

**Mohammad Abbas**
- **Glucose Monitoring & Sequence Design:**  
  Designed the glucose monitoring system and produced corresponding sequence diagrams.
- **Control IQ Implementation:**  
  Developed the automated insulin adjustment features.
- **Safety & Error Handling:**  
  Implemented error detection mechanisms and safety protocols for the pump to operate without error.
- **Integration Testing & Documentation:**  
  Conducted integration tests and updated documentation to reflect the system’s safety operations.

**Aaron McLean**
- **Project Configuration & Data Storage:**  
  Set up the GitHub repository, established init project configuration, and designed the data storage framework.
- **History Logging & Resource Monitoring:**  
  Implemented detailed logging of insulin delivery and system state, so users can track all data from a session. Implementented persistence of the user profile data to remain between sessions, and new builds of the project. 
- **Final Documentation & System Visualization:**  
  Final documentation and authored state machine diagrams.

FILE STRUCTURE
--------------
Root
------
- main.cpp
- COMP3004A4.pro
- README.txt

Headers
-------
- Battery.h
- BolusCalculator.h
- bolus.h
- ControlIQ.h
- CGM.h
- InsulinCartridge.h
- Profile.h
- Pump.h
- PumpHistory.h
- poweroff.h
- UI.h
- authmanager.h
- lockscreen.h
- statusbar.h
- statusmodel.h
- statusbar.h
- optionswindow.h
- QCustomPlot.h
- contentwidget.h
- powerstatemachine.h

Source Files
------------
- Battery.cpp
- bolus.cpp
- BolusCalculator.cpp
- ControlIQ.cpp
- CGM.cpp
- InsulinCartridge.cpp
- Profile.cpp
- Pump.cpp
- PumpHistory.cpp
- poweroff.cpp
- UI.cpp
- authmanager.cpp
- lockscreen.cpp
- statusmodel.cpp
- statusbar.cpp
- QCustomPlot.cpp
- optionswindow.cpp
- contentwidget.cpp
- powerstatemachine.cpp

UI
--
- mainwindow.ui
- statusbar.ui
- lockscreen2.ui
- optionswindow.ui
- poweroff.ui
- contentwidget.ui
- bolus.ui

Main Window
-----------
- mainwindow.cpp
- mainwindow.h

Documentation
-------------
- Comp3004_project_UseCaseModel.pdf
- DesignDecisions.pdf
- TraceMatrix.pdf
- TraceabilityMatrix.pdf
- class_diagram.jpg

State Machine Diagrams
----------------------
- CGM states.png
- cartridge states.png
- pump power.png
- bolus states.png
- glucose monitoring states.png (including low battery)
- contentwidget, glucose targets
