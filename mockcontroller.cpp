#include "mockcontroller.h"


MockController::MockController(ControllerWidget* widget, QObject* parent)
    : QObject(parent), widget(widget)
{
    std::srand(std::time(nullptr));

    connect(widget, &ControllerWidget::turnOnRequest, this, &MockController::onTurnOn);
    connect(widget, &ControllerWidget::turnOffRequest, this, &MockController::onTurnOff);
    connect(widget, &ControllerWidget::desiredTemperatureChanged, this, &MockController::onTemperatureChanged);
    connect(widget, &ControllerWidget::desiredAirFlowChanged, this, &MockController::onAirFlowChanged);

    connect(&simulationTimer, &QTimer::timeout, this, &MockController::simulateStep);
    simulationTimer.setInterval(2000);
}

void MockController::onTurnOn() {
    running = true;
    simulationTimer.start();

    widget->setBlock1Status(BlockStatus::BLOCK_ON);
    widget->setBlock2Status(BlockStatus::BLOCK_ON);
    widget->setBlock3Status(BlockStatus::BLOCK_ON);
}

void MockController::onTurnOff() {
    running = false;
    simulationTimer.stop();

    widget->setBlock1Status(BlockStatus::BLOCK_OFF);
    widget->setBlock2Status(BlockStatus::BLOCK_OFF);
    widget->setBlock3Status(BlockStatus::BLOCK_OFF);
}

void MockController::onTemperatureChanged(int value) {
    currentTemperature = static_cast<double>(value);
}

void MockController::onAirFlowChanged(AirFlowDirection dir) {
    widget->updateAirflowDirection(dir);
}

void MockController::simulateStep() {
    if (!running) return;

    double actualTemp = currentTemperature + ((std::rand() % 5 - 2) * 0.5);
    double humidity = 40.0 + (std::rand() % 20);
    double pressure = 100000 + (std::rand() % 5000);

    widget->updateTemperature(actualTemp);
    widget->updateHumidity(humidity);
    widget->updatePressure(pressure);

    int random1 = std::rand() % 10;
    int random2 = std::rand() % 10;
    int random3 = std::rand() % 10;

    if (random1 == 0) {
        widget->setBlock1Status(BlockStatus::BLOCK_ERROR);
    } else {
        widget->setBlock1Status(BlockStatus::BLOCK_ON);
    }
    if (random2 == 0) {
        widget->setBlock2Status(BlockStatus::BLOCK_ERROR);
    } else {
        widget->setBlock2Status(BlockStatus::BLOCK_ON);
    }
    if (random3 == 0) {
        widget->setBlock3Status(BlockStatus::BLOCK_ERROR);
    } else {
        widget->setBlock3Status(BlockStatus::BLOCK_ON);
    }
}
