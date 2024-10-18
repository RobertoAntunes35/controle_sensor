import UserException from "../../exception/user_exception.js";
import User from '../model/user_model.js';

import * as http_status from '../../../config/constant/http_status.js';

class UserRepository {

    // busca por email
    async findByEmail(email) {
        try {
            let user_find = await User.findOne({ where: { email } });
            if (!user_find) {
                throw new UserException(http_status.BAD_REQUEST, "User not found.");
            }
            return user_find;
        } catch (error) {
            throw new UserException(
                http_status.BAD_REQUEST,
                error.message
            )
        }
    }

    // busca pelo nome 
    async findByName(name) {
        try {
            let user_find = await User.findOne({ where: { name } })
            if (!user_find) {
                throw new UserException(http_status.BAD_REQUEST, "User not found")
            }
            return user_find;
        } catch (error) {
            console.error(error.message)
            throw new UserException(
                http_status.BAD_REQUEST,
                error.message
            )
        }
    }
}

export default new UserRepository();