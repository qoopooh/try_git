from PySide import QtCore, QtGui, QtDeclarative
import sys
    
# The object which will be represented as an item in the list
# Each Item has 2 properties - model and manufacturer, which we will acces from Python
# The QtCore.Property "name" will be used in QML for ListView delegate text 
class ListItem(QtCore.QObject):  
    def __init__(self, model, manufacturer):
        QtCore.QObject.__init__(self)
        self.model = model
        self.manufacturer = manufacturer 

    def _name(self):
        return self.model

    changed = QtCore.Signal()

    name = QtCore.Property(unicode, _name, notify=changed)

# The model contains only one column. It contains ListItem instancies
# To allow modification of the model content we need to implelemnt functions "removeRows" and "addItem"
# from the QtCore.QAbstractListModel. These functions do nothing by default and return false.
# The function "replaceItems" fills the model with completely new list of ListItems
    
class ItemListModel(QtCore.QAbstractListModel):  
    COLUMNS = ('cars',)

    def __init__(self, items):
        QtCore.QAbstractListModel.__init__(self)
        self._items = items
        self.setRoleNames(dict(enumerate(self.COLUMNS)))

    def rowCount(self, parent=QtCore.QModelIndex()):
        return len(self._items)

    def data(self, index, role):
        if index.isValid() and role == self.COLUMNS.index('cars'):
            return self._items[index.row()]
        return None

    def removeRows(self,firstRow,rowCount,parent = QtCore.QModelIndex()):
        self.beginRemoveRows(parent, firstRow, firstRow+rowCount-1)
        while rowCount != 0:
            del self._items[firstRow]
            rowCount -= 1
        self.endRemoveRows()        

    def addItem(self, item):
        self.beginInsertRows(QtCore.QModelIndex(), len(self._items), len(self._items))
        self._items.append(item)
        self.endInsertRows()
        
    def replaceItems(self, items):
        self.removeRows(0,len(self._items))        
        self.beginInsertRows(QtCore.QModelIndex(), len(self._items), len(self._items))
        self._items = items
        self.endInsertRows()

# This is just a simple function which returns a list of ListItems containing car models and their manufacturers
    
def fillCars(filter):
    cars = [['Ascona','Vectra','Omega','Cruze','Octavia','Charger','Fiesta','Focus','Mondeo'],['Opel','Opel','Opel','Chevrolet','Skoda','Dodge','Ford','Ford','Ford']]
    _carList = []
    for i in range(len(cars[0])):
        if filter == "all" or filter == cars[1][i]:
            _carList.append(ListItem(cars[0][i],cars[1][i]))
    return _carList

# The Controller receives signals from QML. It will receive 2 parameters of QObject type:
# the selected item and the model. 
            
class Controller(QtCore.QObject):
    @QtCore.Slot(QtCore.QObject, QtCore.QObject)            
    def itemSelected(self, listItem, listModel): # This function is called from QML when an item is selected
        print 'User clicked on:', listItem.model, 'produced by', listItem.manufacturer
        carList = fillCars(listItem.manufacturer) # Returns a list of ListItems filtered by manufacturer of selected item
        listModel.replaceItems(carList) # Raplaces the model content to only contain cars from one manufacturer

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    view = QtDeclarative.QDeclarativeView()
    view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
    rc = view.rootContext()
    carList = fillCars("all") # Creates a list of ListItems containing all cars            
    listModel = ItemListModel(carList) # Creates and fills the model 
    rc.setContextProperty('listModel', listModel) # Sets the model so it can be used from QML 
    controller = Controller()
    rc.setContextProperty('controller', controller) # Sets the controller so it can be used from QML
    view.setSource('ListView.qml')
    view.show()
    sys.exit(app.exec_())