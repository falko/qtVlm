/**********************************************************************
qtVlm: Virtual Loup de mer GUI
Copyright (C) 2008 - Christophe Thomas aka Oxygen77

http://qtvlm.sf.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Original code: zyGrib: meteorological GRIB file viewer
Copyright (C) 2008 - Jacques Zaninetti - http://zygrib.free.fr

***********************************************************************/

#include <cmath>
#include <QMessageBox>
#include <QDebug>

#include "DialogRoute.h"
#include "Util.h"
#include "MainWindow.h"
#include "DialogGraphicsParams.h"
#include "mycentralwidget.h"
#include "route.h"
#include "boatVLM.h"
#include "Player.h"
#include <QDesktopWidget>
#include "settings.h"
#include <QStandardItemModel>
#include "POI.h"
#include <QItemDelegate>

//-------------------------------------------------------
// ROUTE_Editor: Constructor for edit an existing ROUTE
//-------------------------------------------------------
DialogRoute::DialogRoute(ROUTE *route,myCentralWidget *parent)
    : QDialog(parent)
{
    this->route=route;
    this->parent=parent;
    setupUi(this);
    Util::setFontDialog(this);
    this->resize(this->tabWidget->width()+10,tabWidget->height()+10);
    tabWidget->setParent(0);
    scroll=new QScrollArea(this);
    scroll->resize(tabWidget->size());
    scroll->setWidget(tabWidget);
    QSize mySize=QSize(tabWidget->size().width()+20,tabWidget->size().height()+20);
    QSize screenSize=QApplication::desktop()->screenGeometry().size()*.8;
    if(mySize.height() > screenSize.height())
    {
        mySize.setHeight(screenSize.height());
    }
    if(mySize.width() > screenSize.width())
    {
        mySize.setWidth(screenSize.width());
    }
    this->resize(mySize);
    scroll->resize(mySize);
    inputTraceColor =new InputLineParams(route->getWidth(),route->getColor(),1.6,  QColor(Qt::red),this,0.1,5);
    verticalLayout->addWidget( inputTraceColor);
    setWindowTitle(tr("Parametres Route"));
    editName->setText(route->getName());
    editFrozen->setChecked(route->getFrozen());
    this->speedLossOnTack->setValue(qRound(route->getSpeedLossOnTack()*100.00));


    startFromBoat->setChecked(route->getStartFromBoat());
    startFromMark->setChecked(!route->getStartFromBoat());

    editDateBox->setDateTime(route->getStartTime());

    editCoasts->setChecked(route->getDetectCoasts());
    hidePois->setChecked(route->getHidePois());
    autoRemove->setChecked(route->getAutoRemove());
    autoAt->setChecked(route->getAutoAt());
    vacStep->setValue(route->getMultVac());
    hidden->setChecked(route->getHidden());
    connect(this->btOk,SIGNAL(clicked()),this,SLOT(accept()));
    connect(this->btCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(this->Envoyer,SIGNAL(clicked()),this,SLOT(slotEnvoyer()));
    if(route->getUseVbvmgVlm())
    {
        if(route->getNewVbvmgVlm())
        {
            this->useVbvmgVlm->setCheckState(Qt::PartiallyChecked);
        }
        else
        {
            this->useVbvmgVlm->setCheckState(Qt::Checked);
        }
    }
    else
        this->useVbvmgVlm->setCheckState(Qt::Unchecked);
    //this->useVbvmgVlm->setToolTip("");
    switch(route->getStartTimeOption())
    {
    case 1:
        editVac->setChecked(true);
        break;
    case 2:
        editGrib->setChecked(true);
        break;
    case 3:
        editDate->setChecked(true);
        editDateBox->setEnabled(true);
    }
    this->tabWidget->setCurrentIndex(0);
    int n=0;
    if(parent->getPlayer()->getType()!=BOAT_REAL)
    {
        if(parent->getBoats())
        {
            QListIterator<boatVLM*> i (*parent->getBoats());
            while(i.hasNext())
            {
                boatVLM * acc = i.next();
                if(acc->getStatus())
                {
                    if(acc->getAliasState())
                        editBoat->addItem(acc->getAlias() + "(" + acc->getBoatPseudo() + ")");
                    else
                        editBoat->addItem(acc->getBoatPseudo());
                    if(acc->getId()==route->getBoat()->getId()) editBoat->setCurrentIndex(n);
                    n++;
                }
            }
        }
    }
    else
    {
        this->editVac->setText(tr("Date de la derniere MAJ de la position"));
        editBoat->addItem(parent->getPlayer()->getName());
        editBoat->setEnabled(false);
        this->pilototo->hide();
        this->tabWidget->removeTab(1);
        this->autoRemove->hide();
        this->autoAt->setChecked(false);
        this->autoAt->hide();
    }
#if 1
    this->pilototo->hide();
    //this->tabWidget->removeTab(1);
#endif
    model= new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Date et heure"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Aller vers"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Cap a suivre apres"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Mode"));
    DateBoxDelegate * delegate=new DateBoxDelegate();
    pilotView->setModel(model);
    pilotView->setItemDelegate(delegate);
    pilotView->header()->setAlternatingRowColors(true);
    pilotView->header()->setDefaultAlignment(Qt::AlignCenter|Qt::AlignVCenter);;
    connect(this->defaultOrders,SIGNAL(clicked()),this,SLOT(slotLoadPilototo()));
    connect(this->customOrders,SIGNAL(clicked()),this,SLOT(slotLoadPilototoCustom()));
    this->pilotView->resizeColumnToContents(2);
    this->pilotView->setColumnWidth(1,pilotView->columnWidth(2));
    this->pilotView->resizeColumnToContents(0);
    this->pilotView->setColumnWidth(0,this->pilotView->columnWidth(0)+30);
    this->pilotView->setColumnWidth(3,pilotView->columnWidth(2));
}
DialogRoute::~DialogRoute()
{
}
void DialogRoute::resizeEvent ( QResizeEvent * /*event*/ )
{
    this->scroll->resize(this->size());
}
//---------------------------------------
void DialogRoute::done(int result)
{
    if(result == QDialog::Accepted)
    {
        if (!parent->freeRouteName((editName->text()).trimmed(),route))
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Ce nom est deja utilise, choisissez en un autre"));
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
        route->setSpeedLossOnTack((double)this->speedLossOnTack->value()/100.00);
        route->setBusy(true);
        route->setName((editName->text()).trimmed());
        route->setWidth(inputTraceColor->getLineWidth());
        route->setColor(inputTraceColor->getLineColor());
        if(this->useVbvmgVlm->checkState()==Qt::Unchecked)
        {
            route->setUseVbVmgVlm(false);
            route->setNewVbvmgVlm(false);
        }
        else if(this->useVbvmgVlm->checkState()==Qt::PartiallyChecked)
        {
            route->setUseVbVmgVlm(true);
            route->setNewVbvmgVlm(true);
        }
        else
        {
            route->setUseVbVmgVlm(true);
            route->setNewVbvmgVlm(false);
        }
        route->setAutoRemove(this->autoRemove->isChecked());
        route->setAutoAt(autoAt->isChecked());
        route->setPilototo(this->pilototo->isChecked());
        Settings::setSetting("useVbvmgVlm",route->getUseVbvmgVlm()?"1":"0"  );
        Settings::setSetting("useNewVbvmgVlm",route->getNewVbvmgVlm()?"1":"0"  );
        route->setMultVac(vacStep->value());
        if(editVac->isChecked())
            route->setStartTimeOption(1);
        if (editGrib->isChecked())
            route->setStartTimeOption(2);
        if (editDate->isChecked())
        {
            route->setStartTimeOption(3);
            route->setStartTime(editDateBox->dateTime());
        }
        route->setStartFromBoat(startFromBoat->isChecked());
        if(parent->getPlayer()->getType()!=BOAT_REAL)
        {
            if(parent->getBoats())
            {
                QListIterator<boatVLM*> i (*parent->getBoats());
                while(i.hasNext())
                {
                    boatVLM * acc = i.next();
                    if(acc->getBoatPseudo()==editBoat->currentText())
                    {
                        route->setBoat(acc);
                        break;
                    }
                }
            }
        }
        else
            route->setBoat((boat *) parent->getRealBoat());
        route->setHidden(hidden->isChecked());
        route->setFrozen3(editFrozen->isChecked());
        route->setDetectCoasts(editCoasts->isChecked());
        if(hidePois->isChecked()!=route->getHidePois())
            route->setHidePois(hidePois->isChecked());
        if(this->Simplifier->isChecked())
            route->setSimplify(true);
        else
            route->setSimplify(false);
        route->setBusy(false);
    }
    if(result == QDialog::Rejected)
    {
    }
    QDialog::done(result);
}
void DialogRoute::GybeTack(int i)
{
    QFont font=this->labelTackGybe->font();
    if(i==100)
        font.setBold(false);
    else
        font.setBold(true);
    this->labelTackGybe->setFont(font);
}
void DialogRoute::slotLoadPilototo()
{
    if(!(this->startFromBoat->isChecked() &&
         this->editVac->isChecked() && this->useVbvmgVlm->isChecked()))
    {
        QMessageBox::critical(0,tr("Pilototo"),tr("Pour utiliser cette action il faut que:<br>- La route parte du bateau<br>- La route parte de la prochaine vacation<br>- Le mode VBVMG-VLM soit actif"));
        return;
    }
    if(route->getPoiList().isEmpty())
    {
        return;
    }
    this->fillPilotView(true);
}
void DialogRoute::slotLoadPilototoCustom()
{
    if(!(this->startFromBoat->isChecked() &&
         this->editVac->isChecked() && this->useVbvmgVlm->isChecked()))
    {
        QMessageBox::critical(0,tr("Pilototo"),tr("Pour utiliser cette action il faut que:<br>- La route parte du bateau<br>- La route parte de la prochaine vacation<br>- Le mode VBVMG-VLM soit actif"));
        return;
    }
    if(route->getPoiList().isEmpty())
    {
        return;
    }
    this->fillPilotView(false);
}
void DialogRoute::fillPilotView(bool def)
{
    if(!(route->getStartFromBoat() &&
         route->getStartTimeOption()==1 && route->getUseVbvmgVlm()))
    {
        QMessageBox::critical(0,tr("Pilototo"),tr("Pour utiliser cette action il faut que:<br>- La route parte du bateau<br>- La route parte de la prochaine vacation<br>- Le mode VBVMG-VLM soit actif"));
        return;
    }
    if(route->getPoiList().isEmpty())
    {
        return;
    }
    model->removeRows(0,model->rowCount());
    listPois.clear();
    for(int n=0;n<route->getPoiList().count();++n)
    {
        if(model->rowCount()==6) break;
        POI * poi=route->getPoiList().at(n);
        if(!def && !poi->getPiloteSelected()) continue;
        listPois.append(poi);
        time_t eta;
        QList<QStandardItem*> items;
        if(listPois.count()!=1)
        {
            eta=listPois.at(listPois.count()-2)->getRouteTimeStamp();
            eta=eta+20;
            if(eta==-1) break;
            items.append(new QStandardItem());
            QDateTime tt=QDateTime().fromTime_t(eta).toUTC();
            tt.setTimeSpec(Qt::UTC);
            items[0]->setData(tt.toString("dd MMM yyyy-hh:mm:ss"),Qt::EditRole);
            items[0]->setEditable(true);
        }
        else
        {
            items.append(new QStandardItem(tr("WP-VLM")));
            items[0]->setEditable(false);
        }
        items[0]->setData(QVariant(QMetaType::VoidStar, &poi ),Qt::UserRole);
        items[0]->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
        items.append(new QStandardItem(poi->getName()));
        items[1]->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
        items[1]->setEditable(false);
        items.append(new QStandardItem(QString().sprintf("%.2f",poi->getWph())));
        items[2]->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
        items[2]->setEditable(false);
        switch(poi->getNavMode())
        {
            case 0:
                items.append(new QStandardItem("VB-VMG"));
                break;
            case 1:
                items.append(new QStandardItem("VMG"));
                break;
            case 2:
                items.append(new QStandardItem("ORTHO"));
                break;
        }
        items[3]->setTextAlignment(Qt::AlignCenter| Qt::AlignVCenter);
        items[3]->setEditable(false);
        model->appendRow(items);
    }

    this->pilotView->resizeColumnToContents(2);
    this->pilotView->setColumnWidth(1,pilotView->columnWidth(2));
    this->pilotView->resizeColumnToContents(0);
    this->pilotView->setColumnWidth(0,this->pilotView->columnWidth(0)+30);
    this->pilotView->setColumnWidth(3,pilotView->columnWidth(2));
}
void DialogRoute::slotEnvoyer()
{
    QList<POI*> poiList;
    for (int n=0;n<model->rowCount();++n)
    {
        POI * poi=reinterpret_cast<struct POI *>(qvariant_cast<void*>(model->item(n,0)->data(Qt::UserRole)));
        QDateTime tt=QDateTime().fromString(model->item(n,0)->data(Qt::EditRole).toString(),"dd MMM yyyy-hh:mm:ss");
        tt.setTimeSpec(Qt::UTC);
        //qWarning()<<poi->getName()<<tt;
        poi->setPiloteDate(tt.toTime_t());
        poiList.append(poi);
    }
    parent->setPilototo(poiList);
    this->done(QDialog::Accepted);
}

//---------------------------------------
DateBoxDelegate::DateBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}
QWidget *DateBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QDateTimeEdit *editor = new QDateTimeEdit(parent);
    editor->setTimeSpec(Qt::UTC);
    editor->setDisplayFormat("dd MMM yyyy-hh:mm:ss");
    return editor;
}
void DateBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QDateTime value = QDateTime().fromString(index.model()->data(index, Qt::EditRole).toString(),"dd MMM yyyy-hh:mm:ss");
    value.setTimeSpec(Qt::UTC);
    //qWarning()<<"setEditorData"<<index<<value;
    QDateTimeEdit *editBox = static_cast<QDateTimeEdit*>(editor);
    editBox->setMinimumDateTime(QDateTime().currentDateTimeUtc());
    editBox->setDateTime(value);
}
void DateBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDateTimeEdit *editBox = static_cast<QDateTimeEdit*>(editor);
    QDateTime value = editBox->dateTime().toUTC();
    value.setTimeSpec(Qt::UTC);
    model->setData(index,value.toString("dd MMM yyyy-hh:mm:ss"),Qt::EditRole);
}
void DateBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
