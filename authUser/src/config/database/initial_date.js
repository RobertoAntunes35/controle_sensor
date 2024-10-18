import bcrypt from 'bcrypt';

import User from '../../module/user/model/user_model.js';

export async function createInitialData() {
    try {
        await User.sync({force: true});

        // hash password 
        let password = await bcrypt.hash('123456', 10);
        
        await User.create({
            name: 'Roberto',
            email: 'robertoantunes@email.com',
            password: password
        })
    } catch (err) {
        console.error(err.message)
    }
}