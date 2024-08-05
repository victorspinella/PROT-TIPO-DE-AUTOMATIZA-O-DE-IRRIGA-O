const Estufa = require("../../orm/models/estufaModel");
const Relatorio = require("../../orm/models/relatorioModel");
const { Op } = require("sequelize");

module.exports = {
  async all(request, response) {
    try {
      const relatorio = await Relatorio.findAll({
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
        include: [
          {
            model: Estufa,
            attributes: ['id', 'nome', 'localizacao', 'capacidade'], // Campos da tabela Estufa
          }
        ],
      });
      response.status(200).json(relatorio);
    } catch (error) {
      console.log(error);
      response.status(400).send(error);
    }
  },

  async one(request, response) {
    const dataInicio = new Date(request.query.dataInicio);
    const dataFim = new Date(request.query.dataFim);

    try {
      const relatorios = await Relatorio.findAll({
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
        include: [
          {
            model: Estufa,
            attributes: ['id', 'nome', 'localizacao', 'capacidade'], // Campos da tabela Estufa
          }
        ],
        where: {
          DataLeitura: {
            [Op.between]: [
              dataInicio.toISOString(), // Converter para formato ISO
              dataFim.toISOString(),    // Converter para formato ISO
            ],
          },
        },
      });

      response.status(200).json(relatorios);
    } catch (error) {
      console.log(error);
      response.status(400).send(error);
    }
  },
  
  async delete(request, response) {
    try {
      const id = request.params.id;
      const relatorio = await Relatorio.destroy({ where: { LeituraID: id } });
      if (!relatorio) {
        return response.status(400).json("Data not found");
      }
      response.status(200).json("Data removed!!");
    } catch (error) {
      console.log(error);
      response.status(400).send(error);
    }
  },
  
  formatarDataIsoParaString(dataIso) {
    const dataObj = new Date(dataIso);
    const ano = dataObj.getFullYear();
    const mes = String(dataObj.getMonth() + 1).padStart(2, '0');
    const dia = String(dataObj.getDate()).padStart(2, '0');
    return `${ano}-${mes}-${dia}`;
  }
};
