const { ControleIrrigacao } = require('../../orm/models/statusSistemaModel');

const getUmidadeSolo = async (req, res) => {
  try {
    const dados = await ControleIrrigacao.findAll({
      attributes: ['UmidadeSolo', 'DataLeitura'],
      order: [['DataLeitura', 'DESC']],
      limit: 10
    });
    res.json(dados);
  } catch (error) {
    console.error('Erro ao buscar dados de umidade do solo:', error);
    res.status(500).json({ error: 'Erro ao buscar dados de umidade do solo' });
  }
};

const getUmidadeAmbiente = async (req, res) => {
  try {
    const dados = await ControleIrrigacao.findAll({
      attributes: ['UmidadeAmbiente', 'DataLeitura'],
      order: [['DataLeitura', 'DESC']],
      limit: 30
    });
    res.json(dados);
  } catch (error) {
    console.error('Erro ao buscar dados de umidade do ambiente:', error);
    res.status(500).json({ error: 'Erro ao buscar dados de umidade do ambiente' });
  }
};

const getTemperaturaAmbiente = async (req, res) => {
  try {
    const dados = await ControleIrrigacao.findAll({
      attributes: ['TemperaturaAmbiente', 'DataLeitura'],
      order: [['DataLeitura', 'DESC']],
      limit: 30
    });
    res.json(dados);
  } catch (error) {
    console.error('Erro ao buscar dados de temperatura do ambiente:', error);
    res.status(500).json({ error: 'Erro ao buscar dados de temperatura do ambiente' });
  }
};

module.exports = {
  getUmidadeSolo,
  getUmidadeAmbiente,
  getTemperaturaAmbiente
};
