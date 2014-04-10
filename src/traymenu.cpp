#include "traymenu.h"
#include <QLabel>
#include <QAction>
#include <QApplication>
#include <QWidgetAction>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include "light.h"
TrayMenu::TrayMenu(Light* light, QWidget* parent) :
  QMenu(parent)
{
  QSlider* brightnessSlider = new QSlider(this);
  brightnessSlider->setOrientation(Qt::Horizontal);
  brightnessSlider->setMinimum(0);
  brightnessSlider->setMaximum(255);
  QWidgetAction* brightnessAction = new QWidgetAction(this);
  QLabel* brightnessLabel = new QLabel("Brightness", this);
  QVBoxLayout* brightnessLayout = new QVBoxLayout(this);
  brightnessLayout->addWidget(brightnessLabel);
  brightnessLayout->addWidget(brightnessSlider);
  QWidget* brightnessWidget = new QWidget(this);
  brightnessWidget->setLayout(brightnessLayout);
  brightnessAction->setDefaultWidget(brightnessWidget);
  this->addAction(brightnessAction);


  this->addSeparator();
  QAction* quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  this->addAction(quitAction);
  pLight = light;
  connect(brightnessSlider, SIGNAL(valueChanged(int)),
                     pLight, SLOT(setBrightness(int)), Qt::QueuedConnection);

}

void TrayMenu::brightnessChanged(int newBrightness)
{
  pLight->setBrightness(newBrightness);
}

