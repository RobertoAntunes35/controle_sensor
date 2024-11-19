import jwt from 'jsonwebtoken';
import { promisify } from 'util';

import AuthException from './auth_exception.js';

import * as secret from '../constant/secret.js';
import * as http_status from '../constant/http_status.js';
import * as status_rabbitmq from '../constant/status_rabbitmq.js';
import RabbitmqServer from '../../rabbitmq-server.js';

export default async (req, resp, next) => {
    try {
        const { authorization } = req.headers;
        const server = new RabbitmqServer(status_rabbitmq.URI_RABBIT)
        await server.start();

        if (!authorization) {
            await server.publishInQueueLogs(status_rabbitmq.logs_error, 'SOLICITADO INFORMAÇÃO SEM O TOKEN DE ACESSO.', status_rabbitmq.NA)
            throw new AuthException(http_status.UNAUTHORIZED, 'Access token wat not informed or is wrong')
        }

        const token = authorization.split(" ")[1]

        try {
            const decoded = await promisify(jwt.verify)(token, secret.API_SECRET)
            req.authUser = decoded.authUser;
            return next();
        } catch (err) {
            await server.publishInQueueLogs(status_rabbitmq.logs_error, 'TOKEN INVALIDO', 401)
            console.error('Error verifying JWT: ', err.message)
            resp.status(401).json({
                status: 401,
                message: 'Invalid token'
            });
            return;
        }
    } catch (error) {
        return resp.status(error.status || http_status.INTERNAL_SERVER_ERROR).json({
            status: error.status ? error.status : http_status.INTERNAL_SERVER_ERROR,
            message: error.message
        })
    }
}