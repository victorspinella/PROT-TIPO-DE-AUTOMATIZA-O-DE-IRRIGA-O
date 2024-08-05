'use strict';
const { Model, DataTypes } = require('sequelize');

module.exports = (sequelize) => {
  class ControleIrrigacao extends Model {
    static associate(models) {
      ControleIrrigacao.belongsTo(models.Estufa, {
        foreignKey: 'estufa_id',
        as: 'estufa'
      });
    }
  }
  
  ControleIrrigacao.init({
    estufa_id: {
      type: DataTypes.INTEGER,
      allowNull: false,
      references: {
        model: 'Estufas',
        key: 'id'
      }
    },
    UmidadeSolo: {
      type: DataTypes.FLOAT,
      allowNull: false
    },
    UmidadeAmbiente: {
      type: DataTypes.FLOAT,
      allowNull: false
    },
    TemperaturaAmbiente: {
      type: DataTypes.FLOAT,
      allowNull: false
    },
    DataLeitura: { // Atualizado para DataLeitura
      type: DataTypes.DATE,
      allowNull: false,
      defaultValue: DataTypes.NOW
    }
  }, {
    sequelize,
    modelName: 'ControleIrrigacao',
    tableName: 'ControleIrrigacao',
    timestamps: false
  });

  return ControleIrrigacao;
};
