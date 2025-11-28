#pragma once
#include "App.h"
#include "DataStructures.h" 

class AppManager; 

enum class WorkoutState {
    MAIN_MENU,
    WORKOUT_SELECTION,
    WORKOUT_HISTORY,
    EXERCISE_LIST,
    EXERCISE_VIEW,
    EDIT_VIEW,
    SUMMARY_VIEW
};

struct UiButton {
    int x, y, w, h;
    std::string label;
};

class WorkoutApp : public App {
public:
    WorkoutApp(AppManager* manager);

    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;

private:
    AppManager* appManager;
    WorkoutState currentState;
    
    std::string breadcrumbPath;
    std::vector<std::string> workoutJsonFiles; 
    Workout currentlyLoadedWorkout;
    MuscleGroup currentlyLoadedMuscleGroup;
    Exercise currentlyLoadedExercise;
    int selectedMuscleGroupIndex;
    int selectedExerciseIndex;

    void findWorkoutFiles();
    void loadWorkoutFromFile(const std::string& fileName);
    void loadTodaysLog();
    //void parseMuscleGroups(JsonObject& musclesObject);
    //void parseExercises(JsonArray& exercisesObject);
    bool isFirstTimeFindingFiles = true;

    void saveTodaysLog();
    void updateWorkoutFile();

    void drawMainMenu();
    void drawWorkoutSelection();
    void drawExerciseList();
    void drawExerciseView();
    void drawEditView();
    void drawSummaryView();

    void handleMainMenuTouch(int x, int y);
    void handleWorkoutSelectionTouch(int x, int y);
    void handleExerciseListTouch(int x, int y);
    void handleExerciseViewTouch(int x, int y);
    void handleEditViewTouch(int x, int y);
    std::vector<UiButton> mainMenuButtons;
    std::vector<UiButton> workoutSelectionButtons;
    std::string currentSplit;
};