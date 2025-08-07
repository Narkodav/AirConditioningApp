#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

/**
 * @file controllerwidget.h
 * @brief Defines the ControllerWidget class, which provides a GUI for air conditioner control.
 */

#include <QWidget>
#include <QGraphicsScene>
#include <QSettings>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QGraphicsRectItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFormLayout>
#include <QCheckBox>

/**
 * @enum BlockStatus
 * @brief Represents the current status of a system block.
 */
enum class BlockStatus {
    BLOCK_OFF,    ///< Block is off (gray)
    BLOCK_ERROR,  ///< Block is in error state (red)
    BLOCK_ON      ///< Block is operating normally (green)
};

/**
 * @enum AirFlowDirection
 * @brief Represents the airflow direction mode.
 */
enum class AirFlowDirection {
    AUTO,     ///< Automatic direction
    UP,       ///< Upward airflow
    DOWN,     ///< Downward airflow
    SIDEWAYS  ///< Side airflow
};

/**
 * @enum Theme
 * @brief Represents the UI theme mode.
 */
enum class Theme : bool {
    DARK = true,  ///< Dark mode
    LIGHT = false ///< Light mode
};

/**
 * @class MockController
 * @brief A MockController class declaration so it can be used as a member.
 */
class MockController;

/**
 * @class ControllerWidget
 * @brief A QWidget that simulates and controls an air conditioning system UI.
 */
class ControllerWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ControllerWidget.
     * @param parent Parent QWidget.
     */
    ControllerWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor. Saves current settings.
     */
    ~ControllerWidget();

signals:
    /**
     * @brief Emitted when the system is turned on.
     */
    void turnOnRequest();

    /**
     * @brief Emitted when the system is turned off.
     */
    void turnOffRequest();

    /**
     * @brief Emitted when the user changes the desired airflow.
     * @param dir The new airflow direction.
     */
    void desiredAirFlowChanged(AirFlowDirection dir);

    /**
     * @brief Emitted when the user changes the desired temperature.
     * @param value New temperature in Celsius.
     */
    void desiredTemperatureChanged(int value);

public slots:
    /**
     * @brief Updates the current temperature.
     * @param value The current temperature in Celsius.
     */
    void updateTemperature(double value);

    /**
     * @brief Updates the current airflow direction.
     * @param dir The new airflow direction.
     */
    void updateAirflowDirection(AirFlowDirection dir);

    /**
     * @brief Updates the current atmospheric pressure.
     * @param value The new pressure in Pascals.
     */
    void updatePressure(double value);

    /**
     * @brief Updates the current humidity.
     * @param value Humidity percentage.
     */
    void updateHumidity(double value);

    /**
     * @brief Sets the status of block 1.
     * @param status New status.
     */
    void setBlock1Status(BlockStatus status);

    /**
     * @brief Sets the status of block 2.
     * @param status New status.
     */
    void setBlock2Status(BlockStatus status);

    /**
     * @brief Sets the status of block 3.
     * @param status New status.
     */
    void setBlock3Status(BlockStatus status);

private slots:
private slots:
    /**
     * @brief Toggles the system on or off when the power button is clicked.
     */
    void toggleSystem();

    /**
     * @brief Called when the temperature slider changes.
     * @param value Desired temperature in Celsius.
     */
    void updateTemperatureRequest(int value);

    /**
     * @brief Called when the airflow direction combo box changes.
     * @param index Index corresponding to AirFlowDirection.
     */
    void updateAirflowDirectionRequest(int index);

    /**
     * @brief Called when the temperature unit combo box changes.
     * @param index Selected unit index: 0 = °C, 1 = °F, 2 = K.
     */
    void changeTemperatureUnit(int index);

    /**
     * @brief Called when the pressure unit combo box changes.
     * @param index Selected unit index: 0 = Pa, 1 = mmHg.
     */
    void changePressureUnit(int index);

    /**
     * @brief Switches between dark and light UI themes.
     */
    void toggleTheme();

    /**
     * @brief Opens a dialog to simulate environmental conditions and system state.
     */
    void showSimulationDialog();

    /**
     * @brief Saves user settings (theme, units) to an XML file.
     */
    void saveSettings();

    /**
     * @brief Loads previously saved settings from XML file.
     */
    void loadSettings();

private:
    /**
     * @brief Scene that contains graphical block representations.
     */
    QGraphicsScene *scene;

    /**
     * @brief Graphical rectangles representing system blocks.
     */
    QGraphicsRectItem *block1, *block2, *block3;

    /**
     * @brief Labels for displaying current sensor readings and UI selections.
     */
    QLabel *tempLabel, *humidityLabel, *pressureLabel, *airflowLabel;

    /**
     * @brief Labels for user input section.
     */
    QLabel *tempSelectLabel, *airflowSelectLabel, *unitSelectLabel;

    /**
     * @brief Buttons for power toggle, theme switching, and simulation dialog.
     */
    QPushButton *powerButton, *themeButton, *simulateButton;

    /**
     * @brief Slider for adjusting desired temperature.
     */
    QSlider *tempSlider;

    /**
     * @brief Combo boxes for selecting airflow direction, temperature unit, and pressure unit.
     */
    QComboBox *airflowCombo, *tempUnitCombo, *pressureUnitCombo;

    /**
     * @brief Text items labeling each block in the graphics scene.
     */
    QGraphicsTextItem *block1Label, *block2Label, *block3Label;

    /**
     * @brief Current theme setting.
     */
    Theme theme = Theme::LIGHT;

    /**
     * @brief Indicates whether the system is turned on.
     */
    bool isSystemOn = false;

    /**
     * @brief Current actual temperature in Celsius.
     */
    double currentTempC = 0.0f;

    /**
     * @brief Current atmospheric pressure in Pascals.
     */
    double currentPressurePa = 0.0f;

    /**
     * @brief Current relative humidity percentage.
     */
    double currentHumidity = 0.0f;

    /**
     * @brief Desired temperature set by the user in Celsius.
     */
    double currentDesiredTempC = 0.0f;

    /**
     * @brief Currently selected airflow direction.
     */
    AirFlowDirection currentAirflowSetting = AirFlowDirection::AUTO;

    /**
     * @brief Selected unit for temperature display ("C", "F", or "K").
     */
    QString currentTempUnit = "C";

    /**
     * @brief Selected unit for pressure display ("Pa" or "mmHg").
     */
    QString currentPressureUnit = "Pa";

    /**
     * @brief A mock controller for random imitation
     */
    MockController* controller = nullptr;

    /**
     * @brief Updates all display labels to reflect the current state.
     */
    void updateDisplay();

    /**
     * @brief Converts a temperature from Celsius to the specified unit.
     * @param temp Temperature in Celsius.
     * @param toUnit Target unit ("C", "F", or "K").
     * @return Converted temperature.
     */
    double convertTemperature(double temp, const QString &toUnit);

    /**
     * @brief Converts pressure from Pascals to the specified unit.
     * @param pressure Pressure in Pascals.
     * @param toUnit Target unit ("Pa" or "mmHg").
     * @return Converted pressure.
     */
    double convertPressure(double pressure, const QString &toUnit);

    /**
     * @brief Updates the color of a block based on its status.
     * @param block The graphical block item to update.
     * @param status The new block status.
     */
    void updateBlockColor(QGraphicsRectItem* block, BlockStatus status);
};

#endif // CONTROLLERWIDGET_H
