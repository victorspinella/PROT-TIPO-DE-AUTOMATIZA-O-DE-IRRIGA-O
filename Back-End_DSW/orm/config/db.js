// const Sequelize = require("sequelize");
// const sequelize = new Sequelize("crud-db", "user", "password", {
//   dialect: "sqlite",
//   host: "./dev.sqlite",
//   logging: false
// });

// module.exports = sequelize;

const { Sequelize } = require("sequelize");

const dbUser = process.env.DB_USER;
const dbPass = process.env.DB_PASS;
const dbHost = process.env.DB_HOST;
const dbName = process.env.DB_NAME;

const sequelize = new Sequelize(dbName, dbUser, dbPass, {
  host: dbHost,
  dialect: "mysql",
  logging: false,
});

module.exports = sequelize;
