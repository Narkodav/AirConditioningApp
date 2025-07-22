#include "controllerwidget.h"

ControllerWidget::ControllerWidget(QWidget *parent) : QWidget(parent) {

    // --- Настройка размеров окна ---
    setMinimumSize(800, 600);  // Минимальный размер
    setMaximumSize(1024, 768); // Максимальный размер

    // Фиксируем соотношение сторон (опционально)
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);

    // Graphics Scene for System Status
    scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setFixedSize(600, 200);

    block1 = new QGraphicsRectItem(0, 0, 160, 160);
    block2 = new QGraphicsRectItem(200, 0, 160, 160);
    block3 = new QGraphicsRectItem(400, 0, 160, 160);
    block1->setBrush(Qt::gray);
    block2->setBrush(Qt::gray);
    block3->setBrush(Qt::gray);
    scene->addItem(block1);
    scene->addItem(block2);
    scene->addItem(block3);

    block1Label = scene->addText("Блок 1");
    block1Label->setPos(50, 60);
    block2Label = scene->addText("Блок 2");
    block2Label->setPos(250, 60);
    block3Label = scene->addText("Блок 3");
    block3Label->setPos(450, 60);

    // Controls
    powerButton = new QPushButton("Включить", this);
    powerButton->setCheckable(true);
    powerButton->setStyleSheet("QPushButton { min-width: 100px; min-height: 50px; }");

    tempSlider = new QSlider(Qt::Horizontal, this);
    tempSlider->setRange(16, 30);
    tempSlider->setValue(22);

    airflowCombo = new QComboBox(this);
    airflowCombo->addItems({"Авто", "Вверх", "Вниз", "В стороны"});

    tempUnitCombo = new QComboBox(this);
    tempUnitCombo->addItems({"°C", "°F", "K"});

    pressureUnitCombo = new QComboBox(this);
    pressureUnitCombo->addItems({"Па", "мм.рт.ст"});

    // Labels
    tempLabel = new QLabel("Температура: 22.0 °C", this);
    humidityLabel = new QLabel("Влажность: 50 %", this);
    pressureLabel = new QLabel("Давление: 101325 Па", this);
    airflowLabel = new QLabel("Направление воздуха: Авто", this);

    // Buttons
    themeButton = new QPushButton("Сменить тему", this);
    simulateButton = new QPushButton("Имитация данных", this);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QVBoxLayout *statusLayout = new QVBoxLayout();

    // topLayout->addWidget(view);
    // statusLayout->addWidget(tempLabel);
    // statusLayout->addWidget(humidityLabel);
    // statusLayout->addWidget(pressureLabel);
    // topLayout->addLayout(statusLayout);

    topLayout->addWidget(view);
    topLayout->addWidget(powerButton);

    statusLayout->addWidget(tempLabel);
    statusLayout->addWidget(humidityLabel);
    statusLayout->addWidget(pressureLabel);
    statusLayout->addWidget(airflowLabel);

    view->setAlignment(Qt::AlignHCenter);

    tempSelectLabel = new QLabel("Температура:", this);
    airflowSelectLabel = new QLabel("Направление:", this);
    unitSelectLabel = new QLabel("Единицы измерения:", this);

    controlLayout->addWidget(tempSelectLabel);
    controlLayout->addWidget(tempSlider);
    controlLayout->addWidget(airflowSelectLabel);
    controlLayout->addWidget(airflowCombo);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(statusLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(unitSelectLabel);
    mainLayout->addWidget(tempUnitCombo);
    mainLayout->addWidget(pressureUnitCombo);
    mainLayout->addWidget(themeButton);
    mainLayout->addWidget(simulateButton);

    // Connections
    connect(powerButton, &QPushButton::clicked, this, &ControllerWidget::toggleSystem);
    connect(tempSlider, &QSlider::valueChanged, this, &ControllerWidget::updateTemperatureRequest);
    connect(airflowCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControllerWidget::updateAirflowDirectionRequest);
    connect(tempUnitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControllerWidget::changeTemperatureUnit);
    connect(pressureUnitCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControllerWidget::changePressureUnit);
    connect(themeButton, &QPushButton::clicked, this, &ControllerWidget::toggleTheme);
    connect(simulateButton, &QPushButton::clicked, this, &ControllerWidget::showSimulationDialog);

    updateBlockColor(block1, BlockStatus::BLOCK_OFF);
    updateBlockColor(block2, BlockStatus::BLOCK_OFF);
    updateBlockColor(block3, BlockStatus::BLOCK_OFF);

    QFont font = tempLabel->font();
    font.setPixelSize(20);

    tempLabel->setFont(font);
    humidityLabel->setFont(font);
    pressureLabel->setFont(font);
    tempSelectLabel->setFont(font);
    airflowSelectLabel->setFont(font);
    unitSelectLabel->setFont(font);
    airflowLabel->setFont(font);

    block1Label->setFont(font);
    block2Label->setFont(font);
    block3Label->setFont(font);

    powerButton->setFont(font);
    airflowCombo->setFont(font);
    tempUnitCombo->setFont(font);
    pressureUnitCombo->setFont(font);
    themeButton->setFont(font);
    simulateButton->setFont(font);

    // Load settings
    loadSettings();
}

void ControllerWidget::toggleSystem() {
    isSystemOn = !isSystemOn;
    if(isSystemOn)
    {
        powerButton->setText("Выключить");
        emit(turnOnRequest());
    }
    else
    {
        powerButton->setText("Включить");
        emit(turnOffRequest());
    }
}

void ControllerWidget::updateTemperatureRequest(int value)
{
    currentDesiredTempC = value;
    updateDisplay();
    emit(desiredTemperatureChanged(value));
}

void ControllerWidget::updateAirflowDirectionRequest(int index)
{
    emit(desiredAirFlowChanged(static_cast<AirFlowDirection>(index)));
}

void ControllerWidget::updateTemperature(double value) {
    currentTempC = value;
    updateDisplay();
}

void ControllerWidget::updateAirflowDirection(AirFlowDirection dir) {
    currentAirflowSetting = dir;
    updateDisplay();
}

void ControllerWidget::updatePressure(double value) {
    currentPressurePa = value;
    updateDisplay();

}
void ControllerWidget::updateHumidity(double value) {
    currentHumidity = value;
    updateDisplay();
}

void ControllerWidget::changeTemperatureUnit(int index) {
    QString units[] = {"C", "F", "K"};
    currentTempUnit = units[index];
    updateDisplay();
}

void ControllerWidget::changePressureUnit(int index) {
    currentPressureUnit = (index == 0) ? "Pa" : "mmHg";
    updateDisplay();
}

void ControllerWidget::updateDisplay() {
    double tempDesired = convertTemperature(currentDesiredTempC, currentTempUnit);
    QString tempTextDesired = QString("Температура: %1 %2").arg(tempDesired).arg(currentTempUnit);
    tempSelectLabel->setText(tempTextDesired);

    double temp = convertTemperature(currentTempC, currentTempUnit);
    QString tempText = QString("Температура: %1 %2").arg(temp).arg(currentTempUnit);
    tempLabel->setText(tempText);

    // Давление (имитация)
    double pressure = convertPressure(currentPressurePa, currentPressureUnit);
    pressureLabel->setText(QString("Давление: %1 %2").arg(pressure).arg(currentPressureUnit));
    humidityLabel->setText(QString("Влажность: %1 %").arg(currentHumidity));

    switch(currentAirflowSetting)
    {
    case AirFlowDirection::AUTO:
        airflowLabel->setText(QString("Направление воздуха: Авто"));
        break;
    case AirFlowDirection::UP:
        airflowLabel->setText(QString("Направление воздуха: Вверх"));
        break;
    case AirFlowDirection::DOWN:
        airflowLabel->setText(QString("Направление воздуха: Вниз"));
        break;
    case AirFlowDirection::SIDEWAYS:
        airflowLabel->setText(QString("Направление воздуха: В стороны"));
        break;
    }
}

double ControllerWidget::convertTemperature(double temp, const QString &toUnit) {
    if (toUnit == "F") return temp * 9/5 + 32;
    if (toUnit == "K") return temp + 273.15;
    return temp;
}

double ControllerWidget::convertPressure(double pressure, const QString &toUnit) {
    if (toUnit == "mmHg") return pressure * 0.00750062;
    return pressure;
}

void ControllerWidget::toggleTheme() {
    theme = static_cast<Theme>(!static_cast<bool>(theme));
    setStyleSheet(static_cast<bool>(theme) ? "background: #333; color: white;" : "");
}

void ControllerWidget::showSimulationDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Имитация данных");

    QFormLayout *form = new QFormLayout(&dialog);

    // --- Environment Controls ---
    QSpinBox *tempSpin = new QSpinBox(&dialog);
    tempSpin->setRange(-50, 50);
    tempSpin->setValue(static_cast<int>(currentTempC));
    form->addRow("Температура (°C):", tempSpin);

    QSpinBox *humiditySpin = new QSpinBox(&dialog);
    humiditySpin->setRange(0, 100);
    humiditySpin->setValue(static_cast<int>(currentHumidity));
    form->addRow("Влажность (%):", humiditySpin);

    QSpinBox *pressureSpin = new QSpinBox(&dialog);
    pressureSpin->setRange(70000, 110000);
    pressureSpin->setValue(static_cast<int>(currentPressurePa));
    form->addRow("Давление (Па):", pressureSpin);

    QComboBox *airflowSimCombo = new QComboBox(&dialog);
    airflowSimCombo->addItems({"Авто", "Вверх", "Вниз", "В стороны"});
    airflowSimCombo->setCurrentIndex(static_cast<int>(currentAirflowSetting));
    form->addRow("Направление воздуха:", airflowSimCombo);

    // --- Block Controls ---
    QStringList blockStates = {"Выключен", "Ошибка", "Включен"};

    QComboBox *block1Combo = new QComboBox(&dialog);
    block1Combo->addItems(blockStates);
    form->addRow("Блок 1:", block1Combo);

    QComboBox *block2Combo = new QComboBox(&dialog);
    block2Combo->addItems(blockStates);
    form->addRow("Блок 2:", block2Combo);

    QComboBox *block3Combo = new QComboBox(&dialog);
    block3Combo->addItems(blockStates);
    form->addRow("Блок 3:", block3Combo);

    // --- Buttons ---
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // Use slots to apply values
        updateTemperature(tempSpin->value());
        updateHumidity(humiditySpin->value());
        updatePressure(pressureSpin->value());
        updateAirflowDirection(static_cast<AirFlowDirection>(airflowSimCombo->currentIndex()));

        setBlock1Status(static_cast<BlockStatus>(block1Combo->currentIndex()));
        setBlock2Status(static_cast<BlockStatus>(block2Combo->currentIndex()));
        setBlock3Status(static_cast<BlockStatus>(block3Combo->currentIndex()));
    }
}

void ControllerWidget::saveSettings() {
    QFile file("settings.xml");
    if (!file.open(QIODevice::WriteOnly)) return;

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Settings");
    xml.writeTextElement("TemperatureUnit", currentTempUnit);
    xml.writeTextElement("PressureUnit", currentPressureUnit);
    xml.writeTextElement("Theme", theme == Theme::DARK ? "dark" : "light");
    xml.writeEndElement();
    xml.writeEndDocument();
}

void ControllerWidget::loadSettings() {
    QFile file("settings.xml");
    if (!file.open(QIODevice::ReadOnly)) return;

    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == QString("TemperatureUnit")) {
                currentTempUnit = xml.readElementText();
                tempUnitCombo->setCurrentIndex(currentTempUnit == "F" ? 1 : (currentTempUnit == "K" ? 2 : 0));
            } else if (xml.name() == QString("PressureUnit")) {
                currentPressureUnit = xml.readElementText();
                pressureUnitCombo->setCurrentIndex(currentPressureUnit == "mmHg" ? 1 : 0);
            } else if (xml.name() == QString("Theme") && xml.readElementText() == "dark") {
                toggleTheme();
            }
        }
    }
}

void ControllerWidget::setBlock1Status(BlockStatus status) {
    updateBlockColor(block1, status);
}

void ControllerWidget::setBlock2Status(BlockStatus status) {
    updateBlockColor(block2, status);
}

void ControllerWidget::setBlock3Status(BlockStatus status) {
    updateBlockColor(block3, status);
}

void ControllerWidget::updateBlockColor(QGraphicsRectItem* block, BlockStatus status) {
    switch(status) {
    case BlockStatus::BLOCK_OFF:
        block->setBrush(Qt::gray);
        break;
    case BlockStatus::BLOCK_ERROR:
        block->setBrush(Qt::red);
        break;
    case BlockStatus::BLOCK_ON:
        block->setBrush(Qt::green);
        break;
    }
}

ControllerWidget::~ControllerWidget() {
    saveSettings();
}
