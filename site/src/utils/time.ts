export const getCurrentDate = (): string => {
  const now = new Date();
  const day = String(now.getDate()).padStart(2, "0");       // giorno con 2 cifre
  const month = String(now.getMonth() + 1).padStart(2, "0"); // mese (0-indexed!) quindi +1
  const year = now.getFullYear();

  return `${day}/${month}/${year}`;
};

export const getCurrentTime = (): string => {
  const now = new Date();
  const hours = String(now.getHours()).padStart(2, "0");   // ore 0-23
  const minutes = String(now.getMinutes()).padStart(2, "0"); // minuti 0-59
  return `${hours}:${minutes}`;
};