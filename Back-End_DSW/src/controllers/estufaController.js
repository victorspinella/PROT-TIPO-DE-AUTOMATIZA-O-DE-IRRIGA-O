const Estufa = require("../../orm/models/estufaModel");
const DadosEstufa = require('../../orm/models/dadosEstufaModel');
const ControleIrrigacao = require('../../orm/models/controleIrrigacaoModel');
const sequelize = require("../../orm/config/db");


module.exports = {
  async all(request, response) {
    try {
      const estufaComDadosEstufas = await Estufa.findAll({
        include: [
          {
            model: DadosEstufa,
            attributes: [
              'LeituraID', 
              'EstufaID', 
              'StatusBomba', 
              'StatusValvula', 
              'CorrenteBomba', 
              'UmidadeSolo', 
              'UmidadeAmbiente', 
              'TemperaturaAmbiente', 
              'DataLeitura'
            ],
          }
        ]
      });
      
      response.status(200).json(estufaComDadosEstufas);
    } catch (error) {
      console.log(error);
      response.status(400).send(error);
    }
  },
  
  async create(request, response) {
    const t = await sequelize.transaction();
    try {
      const novaEstufa = await Estufa.create(request.body, { transaction: t });
  
      const dadosEstufa = await DadosEstufa.create({
        EstufaID: novaEstufa.id,
        StatusBomba: 0,
        StatusValvula: 0,
        CorrenteBomba: 0,
        UmidadeSolo: 0,
        UmidadeAmbiente: 0,
        TemperaturaAmbiente: 0,
        DataLeitura: new Date(),
      }, { transaction: t });
  
      await t.commit();
  
      response.status(201).json({
        estufa: novaEstufa,
        dadosEstufa,
      });
    } catch (error) {
      await t.rollback();
  
      console.error(error);
      response.status(400).send(error);
    }
  },

  async delete(request,response){
    const estufaId = request.params.id;

    try {
      const estufa = await Estufa.findByPk(estufaId);

      if (!estufa) {
        return response.status(404).json({ mensagem: 'Estufa não encontrada' });
      }

      await DadosEstufa.destroy({ where: { EstufaID: estufaId } });
      await Estufa.destroy({ where: { id: estufaId } });

      response.status(200).json({ mensagem: 'Estufa e dados associados deletados com sucesso' });
    } catch (error) {
      console.error('Erro ao deletar estufa:', error);
      response.status(500).json({ mensagem: 'Erro interno do servidor' });
    }
  },
}
