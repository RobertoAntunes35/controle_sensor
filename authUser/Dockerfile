FROM node:20.18.0

WORKDIR /app

COPY package.json package-lock.json ./
RUN npm install

COPY . .

CMD ["node", "app.js"]  # Substitua pelo seu arquivo de entrada
