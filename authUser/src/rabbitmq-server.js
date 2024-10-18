
import {connect} from 'amqplib';

export default class RabbitmqServer {
    conn;
    channel;

    constructor(uri) {}

    async start() {
        this.conn = await connect(this.uri);
        this.channel = await this.conn.createChannel();
    }

    async publishInQueueLogs(queue, message, status) {
        let full_message = {
            api: "auth_user",
            message:message,
            status: status
        }
        return await this.channel.sendToQueue(queue, Buffer.from(JSON.stringify(full_message)));
    }

    async publishInQueueAuth(queue, email, token) {
        let full_message = {
            api: "auth_user",
            email:email,
            token: token
        }
        return await this.channel.sendToQueue(queue, Buffer.from(JSON.stringify(full_message)));
    }

    async consume(queue, callback) {
        return await this.channel.consume(queue, (message) => {
            if (message !== null) {
                callback(message.content.toString());
                this.channel.ack(message);
            }
        });
    }
}