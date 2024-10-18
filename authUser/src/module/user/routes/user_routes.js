import { Router } from 'express';

import UserController from '../controller/user_controller.js';
import check_token from '../../../config/auth/check_token.js'
import RabbitmqServer from '../../../rabbitmq-server.js';


const router = new Router();

router.post('/api/v1/user/auth', UserController.getAcessToken);
router.post('/express', async function (req, res, next) {
    const server = new RabbitmqServer('amqp://admin:admin@rabbitmq:5672');
    await server.start();

    await server.publishInQueue('log', 'Oi, tudo bom ?')

    res.send(req.body);
})


router.use(check_token);
router.get('/api/v1/user/email/:email', UserController.findByEmail);
router.get('/api/v1/user/name/:name', UserController.findByName);

export default router;