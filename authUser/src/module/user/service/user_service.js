import UserRepository from '../repository/user_repository.js';
import * as http_status from '../../../config/constant/http_status.js'
import UserException from '../../exception/user_exception.js';
import * as secret from '../../../config/constant/secret.js';
import jwt from 'jsonwebtoken';
import bcrypt from 'bcrypt';
import AuthException from '../../../config/auth/auth_exception.js';


class UserService {

    async findByEmail(req) {
        try {
            // pega o parametro encaminhado pela requisição na url
            const { email } = req.params;
            const { authUser } = req;



            this.validateRequestDataEmail(email);
            let user_find = await UserRepository.findByEmail(email);
            this.validateRequestDataUser(user_find);
            this.validateAuthUser(user_find, authUser);

            return {
                status: http_status.SUCESS,
                user: {
                    id: user_find.id,
                    name: user_find.name
                }
            }
        } catch (error) {
            console.error(error)
            return {
                status: error.status ? error.status : http_status.INTERNAL_SERVER_ERROR,
                message: error.message
            }
        }
    }

    async findByName(req) {
        try {
            const { name } = req.params;
            const { authUser } = req
            this.validateRequestDataName(name);

            let user_find = await UserRepository.findByName(name);
            this.validateRequestDataUser(user_find);
            this.validateAuthUser(user_find, authUser);

            return {
                status: http_status.SUCESS,
                user: {
                    id: user_find.id,
                    name: user_find.name
                }
            }
        } catch (error) {
            console.error(error);
            return {
                status: error.status ? error.status : http_status.INTERNAL_SERVER_ERROR,
                message: error.message
            }
        }
    }
    async getAccessToken(req) {
        try {
            const { email, password } = req.body;

            this.validateDataAccessToken(email, password);

            let user_find = await UserRepository.findByEmail(email);

            await this.validatePassword(password, user_find.password);

            let authUser = {
                id: user_find.id,
                name: user_find.name,
                email: user_find.email
            }

            const accessToken = jwt.sign({ authUser }, secret.API_SECRET, { expiresIn: "1h" })
            return {
                status: http_status.SUCESS,
                accessToken
            }
        } catch (error) {
            return {
                status: error.status ? error.status : http_status.UNAUTHORIZED,
                message: error.message
            }
        }
    }

    // funções complementares
    validateAuthUser(user, authUser) {
        if (!authUser || user.id !== authUser.id) {
            throw new AuthException(http_status.UNAUTHORIZED, "Error to authenticate user")
        }

    }

    async validatePassword(password, hashPassword) {
        console.log("ENTROU VALIDATE PASSWORD")
        if (!await bcrypt.compare(password, hashPassword)) {
            throw new UserException(http_status.UNAUTHORIZED, "Password doens't math")
        }
    }

    validateDataAccessToken(email, password) {
        if (!email || !password) {
            throw new UserException(http_status.UNAUTHORIZED, "Email and password must be informed")
        }
    }

    validateRequestDataUser(user) {
        if (!user) {
            throw new UserException(http_status.BAD_REQUEST, "User wasn't informed")
        }
    }

    validateRequestDataName(name) {
        if (!name) {
            throw new UserException(http_status.INTERNAL_SERVER_ERROR, "User name was not informed")
        }
    }

    validateRequestDataEmail(email) {
        if (!email) {
            throw new UserException(http_status.INTERNAL_SERVER_ERROR, "User email was not informed")
        }
    }
}

export default new UserService();