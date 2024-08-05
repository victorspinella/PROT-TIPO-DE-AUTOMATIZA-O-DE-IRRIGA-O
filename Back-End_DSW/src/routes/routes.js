const express = require('express');
const router = express.Router();

const relatorioController = require('../controllers/relatorioController');
const controlesController = require('../controllers/controlesController');
const estufaController = require('../controllers/estufaController');
const userController = require('../controllers/userController');
const dadosController = require('../controllers/dadosController');


// Rotas para relatórios
router.get('/relatorio', userController.checkToken, relatorioController.all);
router.get('/relatorios',userController.checkToken, relatorioController.one);
router.delete('/deleterelatorio/:id', userController.checkToken, relatorioController.delete);



//Rotas Controles
router.put('/ligar/controles/auto', userController.checkToken, controlesController.ligarControleAuto);
router.put('/desligar/controles/auto', userController.checkToken, controlesController.desligarControleAuto);





//Rotas de Estufas
router.get('/estufas/', userController.checkToken, estufaController.all);
router.delete('/deleteestufa/:id',userController.checkToken, estufaController.delete);
router.post('/addestufa', userController.checkToken, estufaController.create);



//Rotas User
router.post('/auth/register', userController.create);
router.post('/auth/login', userController.login);
//router.post('/auth/logout', userController.logout)

// Rotas para dados dos gráficos
router.get('/umidadeSolo', userController.checkToken, dadosController.getUmidadeSolo);
router.get('/umidadeAmbiente', userController.checkToken, dadosController.getUmidadeAmbiente);
router.get('/temperaturaAmbiente', userController.checkToken, dadosController.getTemperaturaAmbiente);

module.exports = router
