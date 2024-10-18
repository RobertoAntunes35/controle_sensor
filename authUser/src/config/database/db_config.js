// para o vinculo no banco de dados, iremos utilizar o sequelize
import Sequelize  from "sequelize";
import dotenv from 'dotenv';



dotenv.config();
const database = process.env.DB_DATABASE || 'data_base';
const user = process.env.DB_USER || 'root'
const password = process.env.DB_PASSWORD || '123456'

console.log(database);
console.log(user);
console.log(password);



const sequelize = new Sequelize(database, user, password, {
    host: 'localhost',
    port: 3306,
    dialect: 'mysql',
    quoteIdentifiers: false,
    define: {
        syncOnAssociation: true,
        timestamps: false,
        underscored: true,
        underscoredAll: true,
        freezeTableName: true,
    }
})


sequelize.authenticate().then(() => {
    console.info("Connection has ben stablished")
}).catch((err) => {
    console.error("Unable to connect to the database")
    console.info(err.message)
})

export default sequelize;