import { createApp } from 'vue';
import { Quasar, Notify } from 'quasar';
import PrimeVue from 'primevue/config';
import Aura from '@primevue/aura';

// Import Quasar CSS
import '@quasar/extras/material-icons/material-icons.css';
import 'quasar/dist/quasar.css';

import App from './App.vue';
import router from './router';

const app = createApp(App);

app.use(Quasar, {
  plugins: {
    Notify
  },
  config: {
    brand: {
      primary: '#1B4B91',
      secondary: '#26A69A',
      accent: '#9C27B0',
      dark: '#1d1d1d',
      positive: '#21BA45',
      negative: '#C10015',
      info: '#31CCEC',
      warning: '#F2C037'
    },
    notify: {}
  }
});

app.use(router);
app.use(PrimeVue, {
  theme: {
    preset: Aura,
  }
});
app.mount('#app');

