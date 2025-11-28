#include "WorkoutApp.h"
#include "AppManager.h"
#include <SD.h>
#include <ArduinoJson.h> 
#include <iostream>

WorkoutApp::WorkoutApp(AppManager* manager) : appManager(manager) {}

void WorkoutApp::setup() {
    currentState = WorkoutState::MAIN_MENU;
    breadcrumbPath = "Workout";
    findWorkoutFiles();
    // TODO: You could call findWorkoutFiles() here to pre-load the list.
}

void WorkoutApp::update() {
    // This can be used for things that need to happen continuously,
    // like a workout timer in the future.
}

// This function calls the correct drawing function based on the current screen.
void WorkoutApp::draw() {
    M5.Display.clear();
    // TODO: Draw the breadcrumbPath at the top of the screen here.

    switch (currentState) {
        case WorkoutState::MAIN_MENU:
            drawMainMenu();
            break;
        case WorkoutState::WORKOUT_SELECTION:
            drawWorkoutSelection();
            break;
        case WorkoutState::WORKOUT_HISTORY:
            break;
        case WorkoutState::EXERCISE_LIST:
            drawExerciseList();
            break;
        // ... add other cases here
    }
}

// This function calls the correct touch handler based on the current screen.
void WorkoutApp::handleTouch(int x, int y) {
    switch (currentState) {
        case WorkoutState::MAIN_MENU:
            handleMainMenuTouch(x, y);
            break;
        case WorkoutState::WORKOUT_SELECTION:
            handleWorkoutSelectionTouch(x, y);
            break;
        // ... add other cases here
    }
}

// =================================================================
// IMPLEMENTATION STUBS (FILL THESE IN)
// =================================================================

void WorkoutApp::findWorkoutFiles() {
    // TODO:
    // 1. Open the "/workouts" directory on the SD card.
    File workoutsDir = SD.open("/workouts");

    if (workoutsDir && workoutsDir.isDirectory()){
        Serial.println("Workouts directory opened successfully.");

        while (File entry = workoutsDir.openNextFile()){
                std::string tempFileHold = entry.name();
                if(tempFileHold.rfind(".json") == tempFileHold.length() - 5 && tempFileHold[0] != '.'){
                    Serial.println("Found: ");
                    Serial.println(entry.name());
                    workoutJsonFiles.push_back(entry.name());
            }
        }
    }else{
        Serial.println("Failed to open /workouts dir.");
    }
    // 2. Loop through all the files.
    
    // 3. If a file ends with ".json", add its name to the 'workoutJsonFiles' vector.
}

//recursive fucntion approach
/*
void WorkoutApp::parseMuscleGroups(JsonObject& musclesObject){
   JsonArray musclesArray = musclesObject["muscles"].as<JsonArray>(); 
   for(JsonVariant musclesArrayInfo : musclesArray){
        currentlyLoadedMuscleGroup.name = musclesArrayInfo["name"].as<std::string>();
   }
}

void WorkoutApp::parseExercises(JsonArray& exercisesObject){
        for(JsonVariant exercisesInfo : exercisesObject){
            currentlyLoadedExercise.name = exercisesInfo["name"].as<std::string>();
            currentlyLoadedExercise.sets = exercisesInfo["sets"].as<int>();
            currentlyLoadedExercise.reps = exercisesInfo["reps"].as<int>();
        }
}
*/
void WorkoutApp::loadWorkoutFromFile(const std::string& fileName) {
    // TODO:
    // 1. Construct the full path (e.g., "/workouts/" + fileName).
    // 2. Open and read the file from the SD card.
    // 3. Use the ArduinoJson library to parse the text into your 'Workout' struct.
    // 4. Populate the 'currentlyLoadedWorkout' member variable.
    String filePath = "/workouts" + String(fileName.c_str());
    File file = SD.open(filePath);
    if(!file){
        Serial.println("Failed to open file for reading.");
        return;
    }

    //ArduinoJson.h buffer init
    StaticJsonDocument<2048> doc;

    //Deserialization 
    DeserializationError error = deserializeJson(doc, file);
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        file.close(); 
        return;
    }

    file.close();

    //Populate
    JsonObject root = doc.as<JsonObject>();

    currentlyLoadedWorkout.muscleGroups.clear();

    currentlyLoadedWorkout.name = doc["category"].as<std::string>();
    
    //loop to load muscle groups from json :/
    /*
    JsonArray musclesArray = doc["muscles"].as<JsonArray>();
    for(JsonVariant musclesArrayInfo : musclesArray){
        currentlyLoadedMuscleGroup.name = musclesArrayInfo["name"].as<std::string>();

        JsonArray exercisesArray = doc["exercises"].as<JsonArray>();
        for(JsonVariant exercisesArray : exercisesArray){
            currentlyLoadedExercise.name;
        }
        //currentlyLoadedWorkout.muscleGroups.push_back(currentlyLoadedMuscleGroup.name);
    }
    //currentlyLoadedMuscleGroup.name = 
    */
}

void WorkoutApp::drawMainMenu() {
    // This is a good place to define the buttons for this specific screen.
    // This code will run each time the menu is drawn.
    mainMenuButtons.clear();
    mainMenuButtons.push_back({120, 200, 300, 80, "Start Workout"});
    mainMenuButtons.push_back({120, 320, 300, 80, "View History"});

    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawCentreString("Workout App", 270, 100);

    // Set the font for the buttons
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    
    // This loop will draw every button you add to the vector
    for (const auto& button : mainMenuButtons) {
        
        // TODO: Draw the button's filled background rectangle.
        // Use M5.Display.fillRect(x, y, width, height, color).
        // The x, y, width, and height will come from the 'button' object.
        // Use a color like WHITE.
        M5.Display.fillRect(button.x, button.y, button.w, button.h, WHITE);


        // TODO: Draw the button's outline.
        // Use M5.Display.drawRect(x, y, width, height, color).
        // Use the same coordinates and a color like BLACK.
        M5.Display.drawRect(button.x, button.y, button.w, button.h, BLACK);


        // TODO: Draw the button's text label.
        // To center it, first calculate the center coordinates.
        int centerX = button.x + (button.w / 2);
        int centerY = button.y + (button.h / 2) - 8; // Adjust Y slightly for font alignment

        // Now, use M5.Display.drawCentreString(text, centerX, centerY).
        // The text will be button.label.c_str().
        M5.Display.drawCentreString(button.label.c_str(), centerX, centerY);
    }
}

void WorkoutApp::handleMainMenuTouch(int x, int y) {
    // TODO:
    // 1. Define bounding boxes for your two buttons.
    // 2. Check if the touch (x, y) is inside the "Start Workout" button.
    // 3. If it is, change state:
    //    currentState = WorkoutState::WORKOUT_SELECTION;
    //    breadcrumbPath = "Workout/Select";
    //    findWorkoutFiles(); // Load the list of workouts to show next.
    for (const auto& btn : mainMenuButtons) {
        if (x >= btn.x && x <= btn.x + btn.w && y >= btn.y && y <= btn.y + btn.h) {
            Serial.print("Button pressed: ");
            Serial.print(btn.label.c_str());
            if(btn.label == "Start Workout"){
                currentState = WorkoutState::WORKOUT_SELECTION;
            }else if(btn.label == "View History"){
                currentState = WorkoutState::WORKOUT_HISTORY;
            }

            break;
        }
    }
}

void WorkoutApp::drawWorkoutSelection() {
    // TODO:
    // 1. Draw the title "Choose Workout".
    // 2. Loop through the 'workoutJsonFiles' vector.
    // 3. Draw a button for each file name (e.g., "Push", "Pull", "Legs").

    if (isFirstTimeFindingFiles)
    {
        workoutSelectionButtons.clear();

        isFirstTimeFindingFiles = false;
        for (int i = 0; i < workoutJsonFiles.size(); i++)
        {
            int worktSelButnYIncr = 120;
            String nojsonWrktStr = workoutJsonFiles[i].c_str();
            nojsonWrktStr = nojsonWrktStr.substring(0, nojsonWrktStr.length() - 5);

            worktSelButnYIncr *= i;
            workoutSelectionButtons.push_back({120, 200 + worktSelButnYIncr, 300, 80, nojsonWrktStr.c_str()});
            Serial.println("Pushing: ");
            Serial.println(workoutJsonFiles[i].c_str());
        }
    }

    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawCentreString("Choose Workout", 270, 100);
    
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    for(const auto& button : workoutSelectionButtons){

        // TODO: Draw the button's filled background rectangle.
        // Use M5.Display.fillRect(x, y, width, height, color).
        // The x, y, width, and height will come from the 'button' object.
        // Use a color like WHITE.
        M5.Display.fillRect(button.x, button.y, button.w, button.h, WHITE);


        // TODO: Draw the button's outline.
        // Use M5.Display.drawRect(x, y, width, height, color).
        // Use the same coordinates and a color like BLACK.
        M5.Display.drawRect(button.x, button.y, button.w, button.h, BLACK);


        // TODO: Draw the button's text label.
        // To center it, first calculate the center coordinates.
        int centerX = button.x + (button.w / 2);
        int centerY = button.y + (button.h / 2) - 8; // Adjust Y slightly for font alignment

        // Now, use M5.Display.drawCentreString(text, centerX, centerY).
        // The text will be button.label.c_str().
        M5.Display.drawCentreString(button.label.c_str(), centerX, centerY);
    }
}

void WorkoutApp::handleWorkoutSelectionTouch(int x, int y) {
    // TODO:
    // 1. Determine which workout button was pressed based on the y-coordinate.
    // 2. Get the filename from the 'workoutJsonFiles' vector.
    // 3. Call 'loadWorkoutFromFile()' with that filename.
    // 4. Change state: currentState = WorkoutState::EXERCISE_LIST;
    // 5. Update the breadcrumbPath.
    for (const auto& btn : workoutSelectionButtons) {
        if (x >= btn.x && x <= btn.x + btn.w && y >= btn.y && y <= btn.y + btn.h) {
            Serial.println("\nButton pressed: ");
            Serial.print(btn.label.c_str());
            Serial.println();

            currentSplit = btn.label;

            //get split json loaded
            String currentSplitaddJSON = String(currentSplit.c_str()) + ".json";
            loadWorkoutFromFile(currentSplitaddJSON.c_str());

            currentState = WorkoutState::EXERCISE_LIST;

            break;
        }
    }
}

void WorkoutApp::drawExerciseList(){
    String exerciseListTitle = String(currentSplit.c_str());
    //remove 's' plural from end of workout
    if(currentSplit[currentSplit.length() - 1] == 's'){
        String noPluralPlease = exerciseListTitle.substring(0, currentSplit.length() - 1);
        Serial.print("Unpluralised to: ");
        Serial.print(noPluralPlease);
        Serial.println();
        exerciseListTitle = noPluralPlease;
    }
    exerciseListTitle += " Day";
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawCentreString(exerciseListTitle, 270, 100);

    for (int i = 0; i < workoutJsonFiles.size(); i++)
    {
        int worktSelButnYIncr = 120;
        String nojsonWrktStr = workoutJsonFiles[i].c_str();
        nojsonWrktStr = nojsonWrktStr.substring(0, nojsonWrktStr.length() - 5);

        worktSelButnYIncr *= i;
        workoutSelectionButtons.push_back({120, 200 + worktSelButnYIncr, 300, 80, nojsonWrktStr.c_str()});
        Serial.println("Pushing: ");
        Serial.println(workoutJsonFiles[i].c_str());
    }

}
// ... continue implementing the other draw and handle functions ...