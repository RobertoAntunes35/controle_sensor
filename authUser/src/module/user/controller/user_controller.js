import UserService from "../service/user_service.js";


class UserController {

    async findByEmail(req, resp) {
        let user = await UserService.findByEmail(req);
        return resp.status(user.status).json(user);
    }

    async findByName(req, resp) {
        let user = await UserService.findByName(req);
        return resp.status(user.status).json(user);
    }

    async getAcessToken(req, resp) {
        let acessToken = await UserService.getAccessToken(req);
        return resp.status(acessToken.status).json(acessToken);
    }
}

export default new UserController();