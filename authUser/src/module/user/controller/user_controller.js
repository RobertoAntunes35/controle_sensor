import RabbitmqServer from "../../../rabbitmq-server.js";
import UserService from "../service/user_service.js";
import * as status_rabbitmq from '../../../config/constant/status_rabbitmq.js'
class UserController {

    server;

    constructor() {
        this.server = new RabbitmqServer(status_rabbitmq.URI_RABBIT);
    }

    findByEmail = async (req, resp) => {
        const { email } = req.params;
        await this.server.start();
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `SOLICITADO INFORMAÇÕES DO USUARIO PELO EMAIL PARA O EMAIL: ${email}`, status_rabbitmq.NA)
        let user = await UserService.findByEmail(req);
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `STATUS DA SOLICITAÇÃO PARA O EMAIL ${email}`, user.status);
        return resp.status(user.status).json(user);
    }

    findByName = async (req, resp) => {
        const { name } = req.params;
        await this.server.start();
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `SOLICITADO INFORMAÇÕES DO USUARIO PELO NOME PARA O NOME: ${name}`, status_rabbitmq.NA);
        let user = await UserService.findByName(req);
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `STATUS DA SOLICITAÇÃO PARA O NOME: ${name}`, user.status);
        return resp.status(user.status).json(user);
    }

    getAcessToken = async (req, resp) => {
        const { email } = req.body;
        await this.server.start();
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `SOLICITADO ACESSO NA API DE AUTENTICAÇÃO PARA O USUÁRIO DE EMAIL: ${email}`, status_rabbitmq.NA);
        let acessToken = await UserService.getAccessToken(req);
        await this.server.publishInQueueLogs(status_rabbitmq.logs_info, `STATUS DA AUTENTICAÇÃO PARA O EMAIL ${email}`, acessToken.status);
        // se o usuario foi devidamente autenticado, eu menciono para o rabbit que o mesmo esta logado. 
        if (acessToken.status === 200) {
            await this.server.publishInQueueAuth(status_rabbitmq.auth, email, acessToken.accessToken);
        }
        return resp.status(acessToken.status).json(acessToken);
    }
}

export default new UserController();