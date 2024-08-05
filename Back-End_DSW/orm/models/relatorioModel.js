const { Model, DataTypes } = require("sequelize");
const sequelize = require("../config/db");

class Relatorio extends Model {}

Relatorio.init(
  {
    LeituraID: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      autoIncrement: true,
    },
    EstufaID: {
      type: DataTypes.INTEGER,
    },
    StatusBomba: {
      type: DataTypes.BOOLEAN,
    },
    StatusValvula: {
      type: DataTypes.BOOLEAN,
    },
    CorrenteBomba: {
      type: DataTypes.STRING,
    },
    UmidadeSolo: {
      type: DataTypes.STRING,
    },
    UmidadeAmbiente: {
      type: DataTypes.STRING,
    },
    TemperaturaAmbiente: {
      type: DataTypes.STRING,
    },
    DataLeitura: {
      type: DataTypes.DATE,
    },
  },
  {
    sequelize,
    modelName: 'Relatorio',
    tableName: 'ControleIrrigacao', 
    timestamps: false,
  }
);

module.exports = Relatorio;
