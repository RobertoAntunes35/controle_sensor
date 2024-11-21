import React, { useState } from "react";
import { Card, CardContent, Typography, Grid, Button, CircularProgress } from "@mui/material";

const SensorControl = ({ authToken }) => {
  const [sensorData, setSensorData] = useState([]);
  const [loading, setLoading] = useState(false);

  const handleSensorRequest = async (sensorId) => {
    if (!authToken) {
      alert('Não foi possível encontrar o token de autenticação, certifique-se que você esta logado.')
      console.error("Token de autenticação não está disponível!");
      return;
    }

    setLoading(true);
    try {
      const response = await fetch(`http://192.168.0.111:5000/api/v1/get-all-data/${sensorId}`, {
        method: "GET",
        headers: {
          Authorization: `Bearer ${authToken}`, 
        },
      });

      if (!response.ok) {
        alert('Você precisa estar autenticado para realizar a visualização dos dados.')
        throw new Error(`Erro ao acessar o sensor ${sensorId}: ${response.status}`);
      }

      const data = await response.json();
      console.log(`Dados do sensor ${sensorId}:`, data);
      setSensorData(data.data); // Salva os dados dos sensores na state
    } catch (error) {
      console.error("Erro na requisição ao sensor:", error);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div style={{ padding: "20px"}}>
      <div style={{ marginBottom: "20px" }}>
        <Button variant="contained" color="primary" onClick={() => handleSensorRequest('temperatura')} style={{ marginRight: "10px" }}>
          Sensor 1
        </Button>
        <Button variant="contained" color="secondary" onClick={() => handleSensorRequest('distancia')}>
          Sensor 2
        </Button>
      </div>

      {loading ? (
        <CircularProgress color="primary" />
      ) : (
        <Grid container spacing={2}>
          {sensorData.map((sensor) => (
            <Grid item xs={12} sm={6} md={4} key={sensor.id}>
              <Card sx={{ maxWidth: 345 }}>
                <CardContent>
                  <Typography variant="h6" gutterBottom>
                    {sensor.name}
                  </Typography>
                  <Typography variant="body2" color="text.secondary" paragraph>
                    <strong>ID:</strong> {sensor.id}
                  </Typography>
                  <Typography variant="body2" color="text.secondary" paragraph>
                    <strong>Timestamp:</strong> {sensor.timestamp}
                  </Typography>
                  <Typography variant="body2" color="text.secondary">
                    <strong>Value:</strong> {sensor.value}
                  </Typography>
                </CardContent>
              </Card>
            </Grid>
          ))}
        </Grid>
      )}
    </div>
  );
};

export default SensorControl;
