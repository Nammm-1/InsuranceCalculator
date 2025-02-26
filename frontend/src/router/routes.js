import Calculator from 'pages/newCalculator.vue'
import BulkCalculator from 'pages/BulkCalculator.vue'

const routes = [
  {
    path: '/',
    component: () => import('layouts/MainLayout.vue'),
    children: [
      { path: '', component: () => import('pages/Landing.vue') },
      { path: 'ind', component: Calculator },
      { path: 'bulk-calculator', component: () => import('pages/BulkCalculator.vue') },
    ]
  },

  // Always leave this as last one,
  // but you can also remove it
  {
    path: '/:catchAll(.*)*',
    component: () => import('pages/ErrorNotFound.vue')
  }
]

export default routes
