import Sequelize from "sequelize";
import dotenv from 'dotenv';

dotenv.config();

const database = process.env.DB_DATABASE || 'data_base';
const user = process.env.DB_USER || 'root';
const password = process.env.DB_PASSWORD || '123456';
const host = process.env.DB_HOST || 'localhost';
const port = process.env.DB_PORT || 3306;

console.log(database);
console.log(user);
console.log(password);
console.log(host);
console.log(port);

const sequelize = new Sequelize(database, user, password, {
    host: host,
    port: 3306,
    dialect: 'mysql',
    quoteIdentifiers: false,
    define: {
        syncOnAssociation: true,
        timestamps: false,
        underscored: true,
        underscoredAll: true,
        freezeTableName: true,
    },
    retry: {
        max: 5, // Número máximo de tentativas
        match: [
            /ECONNREFUSED/, // Erro de conexão recusada
            /ETIMEDOUT/,    // Timeout de rede
            /ESOCKETTIMEDOUT/, // Timeout de socket
            /ER_ACCESS_DENIED_ERROR/ // Erro de acesso negado
        ],
        min: 1000, // Tempo mínimo entre as tentativas (1 segundo)
        maxTimeout: 10000, // Tempo máximo de timeout entre tentativas (10 segundos)
    }
});

sequelize.authenticate().then(() => {
    console.info("Connection has been established successfully.");
}).catch((err) => {
    console.error("Unable to connect to the database.");
    console.info(err.message);
});

export default sequelize;
