const { Model, DataTypes } = require("sequelize");
const sequelize = require("../config/db");

class DadosEstufa extends Model {}

DadosEstufa.init(
  {
    LeituraID: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      autoIncrement: true
    },
    EstufaID: {
      type: DataTypes.INTEGER,
    },
    StatusBomba: {
      type: DataTypes.BOOLEAN, // ou INTEGER se preferir manter como 0/1
    },
    StatusValvula: {
      type: DataTypes.BOOLEAN, // ou INTEGER se preferir manter como 0/1
    },
    CorrenteBomba: {
      type: DataTypes.DECIMAL,
    },
    UmidadeSolo: {
      type: DataTypes.DECIMAL,
    },
    UmidadeAmbiente: {
      type: DataTypes.DECIMAL,
    },
    TemperaturaAmbiente: {
      type: DataTypes.DECIMAL,
    },
    DataLeitura: {
      type: DataTypes.DATE,
    },
  },
  {
    sequelize,
    modelName: "dadosEstufa",
    timestamps: false 
  }
);

module.exports = DadosEstufa;
