import { Router } from 'express';

import UserController from '../controller/user_controller.js';
import check_token from '../../../config/auth/check_token.js'


const router = new Router();

router.post('/api/v1/user/auth', UserController.getAcessToken);

router.use(check_token);
router.get('/api/v1/user/email/:email', UserController.findByEmail);
router.get('/api/v1/user/name/:name', UserController.findByName);

export default router;