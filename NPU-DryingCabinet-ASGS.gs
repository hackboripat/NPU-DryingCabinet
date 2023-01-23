function myFunction() {
  
}

function doGet(e) { 
  Logger.log( JSON.stringify(e) );
  var result = 'Ok';
  if (e.parameter == 'undefined') {
  result = 'No Parameters';
  }

  else {

    var rowData = [];

    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);

      Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        
        case 'Date':
        rowData[0] = value; // Temperature in column A
        result = 'name_user Written on column A'; 
        break;

        case 'Time':
        rowData[1] = value; // Humidity in column B
        result += ' ,number_user Written on column B'; 
        break; 

        case 'kW':
        rowData[2] = value; // Humidity in column C
        result += ' ,datatest Written on column C'; 
        break; 

        case 'TempIn':
        rowData[3] = value; // Humidity in column D
        result += ' ,datatest Written on column D'; 
        break; 

        case 'HumidityIn':
        rowData[4] = value; // Humidity in column E
        result += ' ,datatest Written on column E'; 
        break; 

        case 'EnthalpyIn':
        rowData[5] = value; // Humidity in column F
        result += ' ,datatest Written on column F'; 
        break; 

        case 'TempOut':
        rowData[6] = value; // Humidity in column G
        result += ' ,datatest Written on column G'; 
        break; 

        case 'HumidityOut':
        rowData[7] = value; // Humidity in column H
        result += ' ,datatest Written on column H'; 
        break; 

        case 'EnthalpyOut':
        rowData[8] = value; // Humidity in column I
        result += ' ,datatest Written on column I'; 
        break; 

        default:
        result = "unsupported parameter";
      }
    }

    var sheet = SpreadsheetApp.getActiveSpreadsheet();
    let NameSheet = String(rowData[0]);
    var yourNewSheet = sheet.getSheetByName(NameSheet);

    if (yourNewSheet == null) {
        // activeSpreadsheet.deleteSheet(yourNewSheet);
        yourNewSheet = sheet.insertSheet();
        yourNewSheet.setName(NameSheet);

        var newRowtitle = yourNewSheet.getLastRow() + 1; 
        var rowtitle = ["Date","Time","Electric Power (W)","Inside Temperature ","Inside Humidity","Inside Enthalpy","Outside Temperature","Outside Humidity","Outside Enthalpy"];

        Logger.log(JSON.stringify(rowtitle));
        let title_newRange = yourNewSheet.getRange(newRowtitle, 1, 1, rowtitle.length);
        title_newRange.setValues([rowtitle]);

    }

    var newRow = yourNewSheet.getLastRow() + 1; 


    Logger.log(JSON.stringify(rowData));
    var newRange = yourNewSheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);

  }
  return ContentService.createTextOutput(result);
}

function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
