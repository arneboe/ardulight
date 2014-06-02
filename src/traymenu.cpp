#include "TrayMenu.h"
#include <QAction>
#include <QWidgetAction>
#include "Light.h"
#include <QDir>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QPluginLoader>
#include "LightController.h"

TrayMenu::TrayMenu(std::shared_ptr<ILight> light, QWidget* parent) :
  QMenu(parent), quitAction(new QAction("Quit", this)), activeController(-1),
  light(light)
{
  //FIXME this should not be connected to qApp.quit(). Instead it should
  //invoke a local slot which cleans up before quitting.
  connect(quitAction, SIGNAL(triggered()),qApp,SLOT(quit()));

  //load all light controllers
  loadPlugins();

  //populate the controller selection submenu
  controllerSelection = new QMenu("Select Mode", this);
  this->addMenu(controllerSelection);
  for(int i = 0; i < controllers.size(); ++i)
  {
    QAction* act = new QAction(this);
    act->setCheckable(true);
    act->setChecked(false);
    act->setData(QVariant(i));
    act->setText(controllers[i]->getName());
    controllerSelection->addAction(act);
  }
  connect(controllerSelection, SIGNAL(triggered(QAction*)),
                    this, SLOT(controllerSelected(QAction*)));

  this->addSeparator();

  //set brightness action
  QSlider* brightnessSlider = new QSlider(nullptr);
  brightnessSlider->setOrientation(Qt::Horizontal);
  brightnessSlider->setMinimum(0);
  brightnessSlider->setMaximum(255);
  brightnessSlider->setValue(255);
  QLabel* brightnesslabel = new QLabel("Brightness", nullptr);
  QHBoxLayout* brightnessLayout = new QHBoxLayout(nullptr);
  brightnessLayout->addWidget(brightnesslabel);
  brightnessLayout->addWidget(brightnessSlider);
  QWidget* brightnessWidget = new QWidget(nullptr);
  brightnessWidget->setLayout(brightnessLayout);
  QWidgetAction* brigthnessAction = new QWidgetAction(nullptr);
  brigthnessAction->setDefaultWidget(brightnessWidget);

  connect(brightnessSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(brightnessChanged(int)));
  this->addAction(brigthnessAction);
  this->addSeparator();
  this->addAction(quitAction);
  if(controllers.size() > 0)
  {
    activateController(0);//FIXME should be loaded from settings?!
    controllerSelection->actions()[0]->setChecked(true);//mark the first controller as active
  }
  else
  {
    qWarning() << "No controllers found!";
  }
}

void TrayMenu::activateController(const int controllerIndex)
{
  Q_ASSERT(controllerIndex >= 0);
  Q_ASSERT(controllerIndex < controllers.size());
  if(activeController != -1)//is -1 if there has never been an active controller
  {
    controllers[activeController]->deactivate();
    this->removeAction(this->actions()[0]);//0 is the topmost action, i.e. the action of the old controller
    //TODO remove old action
  }

  activeController = controllerIndex;
  controllers[controllerIndex]->activate(light);
  Q_ASSERT(this->actions().size() > 0);
  this->insertAction( this->actions()[0], controllers[controllerIndex]->getMenuWidget());
}

void TrayMenu::brightnessChanged(int newBrightness)
{
  Q_ASSERT(newBrightness >= 0);
  Q_ASSERT(newBrightness <= 255);

  if(activeController != -1)//if there is an active controller
  {
    controllers[activeController]->setBrightness(newBrightness);
  }
}

void TrayMenu::controllerSelected(QAction *action)
{
  const int controllerId = action->data().toInt();
  Q_ASSERT(controllerId >= 0);
  Q_ASSERT(controllerId < controllers.size());

  if(controllerId != activeController)
  {
    //uncheck all actions
    for(QAction* action : controllerSelection->actions())
    {
      action->setChecked(false);
    }
    activateController(controllerId);
  }
  controllerSelection->actions()[controllerId]->setChecked(true);
}

void TrayMenu::loadPlugins()
{
  QDir pluginsDir = QDir(qApp->applicationDirPath());
  pluginsDir.cd("plugins");
  qDebug() << "Searching for plugins in: " << pluginsDir.absolutePath();
  for(QString fileName : pluginsDir.entryList(QDir::Files))
  {
    qDebug() << "Loading plugin: " << fileName;
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    if (plugin)
    {
      LightController* controller = qobject_cast<LightController*>(plugin);
      if(controller)
      {
       // std::unique_ptr<LightController> pCtrl(controller);
        controllers.emplace_back(controller);
      }
      else
      {
        qWarning() << "unable to load plugin " << fileName;
      }
    }
    else
    {
      qWarning() << "unable to load plugin " << fileName <<
                    "Error: " << loader.errorString();
    }
  }
}
