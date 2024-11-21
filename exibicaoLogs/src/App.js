import React, { useState } from "react";
import { Button, Container, Grid, Box, Typography } from "@mui/material";
import LogViewer from "./LogViewer";
import SensorControl from "./SensorControl";

const App = () => {
  const [authToken, setAuthToken] = useState(""); // Estado para armazenar o token
  const [selectedView, setSelectedView] = useState("sensor1"); // Estado para controlar qual componente será exibido

  // Função chamada pelo LogViewer ao receber o token
  const handleAuthTokenReceived = (token) => {
    console.log("Token recebido no App:", token);
    setAuthToken(token); // Armazena o token no estado
  };

  const handleButtonClick = (view) => {
    setSelectedView(view); // Muda a visualização com base no botão clicado
  };

  return (
    <Container maxWidth="lg" style={{ paddingTop: "20px" }}>
      {/* Cabeçalho e Botões de Navegação */}
      <Box display="flex" justifyContent="center" mb={4}>
        <Button
          variant={selectedView === "logs" ? "contained" : "outlined"}
          color="primary"
          onClick={() => handleButtonClick("logs")}
          style={{ marginRight: "10px" }}
        >
          Logs
        </Button>
        <Button
          variant={selectedView === "sensores" ? "contained" : "outlined"}
          color="secondary"
          onClick={() => handleButtonClick("sensores")}
          style={{ marginRight: "10px" }}
        >
          Sensores
        </Button>
      </Box>

      {/* Exibição Condicional dos Componentes */}
      {selectedView === "logs" && (
        <LogViewer onAuthTokenReceived={handleAuthTokenReceived} />
      )}

      {selectedView === "sensores" ? (
        <SensorControl authToken={authToken} />
      ) : null}
    </Container>
  );
};

export default App;
