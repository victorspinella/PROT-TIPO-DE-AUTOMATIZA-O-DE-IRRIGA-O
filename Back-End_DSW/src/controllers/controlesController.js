const Controle = require("../../orm/models/controleIrrigacaoModel.js");

class controlesController {
    async ligarControle(request, response) {
        try {
            const id = request.params.id;

            const controle = await Controle.findOne({ where: { id } });

            if (!controle) {
                return response.status(400).json("Data not found");
            }

            controle.comando_bomba = true;
            controle.comando_valvula = true;

            await controle.save();
            response.status(200).json("Ligados");
        } catch (error) {
            console.error(error);
            response.status(500).send(error);
        }
    }

    async desligarControle(request, response) {
        try {
            const id = request.params.id;

            const controle = await Controle.findOne({ where: { id } });

            if (!controle) {
                return response.status(400).json("Data not found");
            }

            controle.comando_bomba = false;
            controle.comando_valvula = false;

            await controle.save();
            response.status(200).json("Desligados");
        } catch (error) {
            console.error(error);
            response.status(500).send(error);
        }
    }

    async listarControles(request, response) {
        try {
            const controle = await Controle.findAll();
            response.status(200).json(controle);
        } catch (error) {
            console.log(error);
            response.status(400).send(error);
        }
    }

    async ligarControleAuto(request, response) {
        try {
            // Lógica para ligar a irrigação automática
            // Pode incluir lógica específica para controle automático
            response.status(200).json({ message: 'Irrigação automática ligada' });
        } catch (error) {
            console.error('Erro ao ligar irrigação automática:', error);
            response.status(500).json({ error: 'Erro ao ligar irrigação automática' });
        }
    }

    async desligarControleAuto(request, response) {
        try {
            // Lógica para desligar a irrigação automática
            // Pode incluir lógica específica para controle automático
            response.status(200).json({ message: 'Irrigação automática desligada' });
        } catch (error) {
            console.error('Erro ao desligar irrigação automática:', error);
            response.status(500).json({ error: 'Erro ao desligar irrigação automática' });
        }
    }
}

module.exports = new controlesController();
