#ifndef MOCKCONTROLLER_H
#define MOCKCONTROLLER_H

/**
 * @file mockcontroller.h
 * @brief Simulates a backend controller for ControllerWidget.
 */

#include <QObject>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include "controllerwidget.h"

/**
 * @class MockController
 * @brief A mock class that simulates the behavior of an AC controller.
 */
class MockController : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param widget The ControllerWidget this controller manages.
     * @param parent Optional parent.
     */
    explicit MockController(ControllerWidget* widget, QObject* parent = nullptr);

public slots:
    /**
     * @brief Handles system start request.
     */
    void onTurnOn();

    /**
     * @brief Handles system stop request.
     */
    void onTurnOff();

private slots:

    /**
     * @brief Handles desired temperature change.
     * @param value New desired temperature.
     */
    void onTemperatureChanged(int value);

    /**
     * @brief Handles desired airflow direction change.
     * @param dir New airflow direction.
     */
    void onAirFlowChanged(AirFlowDirection dir);

    /**
     * @brief Performs a simulation step and updates widget.
     */
    void simulateStep();

private:
    ControllerWidget* widget;  ///< The widget being controlled.
    QTimer simulationTimer;    ///< Timer to trigger periodic simulation updates.
    bool running = false;      ///< Whether the system is active.
    double currentTemperature = 22.0; ///< Target temperature.
};

#endif // MOCKCONTROLLER_H
