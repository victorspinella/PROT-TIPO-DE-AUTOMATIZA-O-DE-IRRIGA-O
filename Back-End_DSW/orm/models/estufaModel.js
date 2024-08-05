const { Model, DataTypes } = require("sequelize");
const sequelize = require("../config/db");
const DadosEstufa = require('../../orm/models/dadosEstufaModel');
const Relatorios = require('../../orm/models/relatorioModel');

class Estufa extends Model {}

Estufa.init(
  {
    nome: {
      type: DataTypes.STRING,
    },
    localizacao: {
      type: DataTypes.STRING,
    },
    capacidade: {
      type: DataTypes.DECIMAL,
    },
  },
  {
    sequelize,
    modelName: "estufas",
    timestamps: false 
  }
);

Estufa.hasOne(DadosEstufa, { foreignKey: 'EstufaID' });
DadosEstufa.belongsTo(Estufa, { foreignKey: 'EstufaID' });
Relatorios.belongsTo(Estufa, { foreignKey: 'EstufaID' });

module.exports = Estufa;
