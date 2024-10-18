import jwt from 'jsonwebtoken';
import { promisify } from 'util';

import AuthException from './auth_exception.js';

import * as secret from '../constant/secret.js';
import * as http_status from '../constant/http_status.js';

export default async (req, resp, next) => {
    try {
        const { authorization } = req.headers;


        console.log('authorization');

        if (!authorization) {
            throw new AuthException(http_status.UNAUTHORIZED, 'Access token wat not informed or is wrong')
        }

        const token = authorization.split(" ")[1]

        try {
            const decoded = await promisify(jwt.verify)(token, secret.API_SECRET)
            req.authUser = decoded.authUser;
            return next();
        } catch (err) {
            console.error('Error verifying JWT: ', err.message)
            resp.status(401).json({
                status: 401,
                message: 'Invalid token'
            });
            return;
        }
    } catch (error) {
        return resp.status(error.status).json({
            status: error.status ? error.status : http_status.INTERNAL_SERVER_ERROR,
            message: error.message
        })
    }
}