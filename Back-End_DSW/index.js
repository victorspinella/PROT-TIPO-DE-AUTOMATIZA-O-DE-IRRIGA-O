require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mongoose = require('mongoose');
const mysql = require('mysql2');
const sequelize = require("./orm/config/db");
const router = require('./src/routes/routes');
const app = express();

app.use(cors());
app.use(express.json());
app.use(router);

const dbUser = process.env.DB_USER;
const dbPass = process.env.DB_PASS;
const dbHost = process.env.DB_HOST;
const dbName = process.env.DB_NAME;

const mongoUser = process.env.MONGO_USER;
const mongoPass = process.env.MONGO_PASS;
// const mongoCluster = process.env.MONGO_CLUSTER;
// const mongoDbName = process.env.MONGO_DB_NAME;

// Conexão ao MongoDB
const mongoosePort = 5000;
mongoose
    .connect(`mongodb+srv://${mongoUser}:${mongoPass}@cluster0.jeocikq.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0`)
    // .connect(`mongodb+srv://victorsm:${dbPass}@cluster0.jeocikq.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0`)
    .then(() => {
        app.listen(mongoosePort, () => {
            console.log(`MONGOOSE Conectado - Porta ${mongoosePort}`);
        });
    })
    .catch((error) => console.log(error));




// Conexão ao MySQL (db4free)
const db = mysql.createConnection({
    host: dbHost,
    user: dbUser,
    password: dbPass,
    database: dbName
  });
  
  db.connect((err) => {
    if (err) {
      console.error('Erro ao conectar ao banco de dados MySQL:', err);
      return;
    }
    console.log('Conectado ao banco de dados MySQL');
  });

  // Conexão ao Sequelize
  sequelize.sync({ force: false }).then(() => {
    console.log("Banco de dados sincronizado");
  }).catch((error) => {
    console.error("Erro ao sincronizar o banco de dados:", error);
  });
    
  const port = 4000;
  app.listen(port, () => {
    console.log(`APP conectado - Porta ${port}`);
  });
  
  // Rota raiz para verificar se o servidor está funcionando
  app.get('/', (req, res) => {
    res.send('Servidor está funcionando!');
  });
  
  // Exemplo de rota para acessar dados do MySQL
  app.get('/tccarduino', (req, res) => {
    const query = 'SELECT * FROM `ControleIrrigacao`';
    
    db.query(query, (err, results) => {
      if (err) {
        console.error('Erro ao executar consulta:', err);
        res.status(500).send('Erro ao executar consulta');
        return;
      }
      res.json(results);
    });
  });




// require('dotenv').config()
// const express = require('express')
// const cors = require('cors')
// const mongoose = require('mongoose')
// const sequelize = require("./orm/config/db")
// const router = require('./src/routes/routes')
// const app = express();


// app.use(cors())
// app.use(express.json())
// app.use(router)


// const dbUser = process.env.DB_USER
// const dbPass = process.env.DB_PASS


// const mongoosePort = 5000
// mongoose
//     .connect(`mongodb+srv://victorsm:${dbPass}@cluster0.jeocikq.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0`)
//     .then(() => {
//         app.listen(mongoosePort)
//         console.log(`MONGOOSE Conectado - Porta ${mongoosePort}`)
//     })
// .catch((error) => console.log(error))








// sequelize.sync().then(() => console.log("SEQUELIZE Conectado"));

// const port = 4000;

// app.listen(port,()=>{
//     console.log(`APP conectado - Porta ${port}`)
// })

