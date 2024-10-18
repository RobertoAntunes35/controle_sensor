import express from 'express';
import * as db from './src/config/database/initial_date.js';
import cors from 'cors';
import user_routes from './src/module/user/routes/user_routes.js'
import check_token from './src/config/auth/check_token.js'


const app = express();
const env = process.env;
const PORT = env.PORT || 8080;

db.createInitialData();
app.use(express.json());
app.use(cors());
app.use(user_routes);

app.get("/api/v1/", (req, res) => {
    return res.status(200).json({
        service: "auth-user",
        status: "Up",
        httpStatus: 200
    })
})

app.listen(PORT, () => {
    console.info(`Server started at ${PORT}`)
})





