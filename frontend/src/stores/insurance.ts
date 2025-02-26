import { create } from 'zustand';
import type { Calculator } from '@shared/schema';
import { apiRequest } from '@/lib/queryClient';

export interface PremiumResult {
  premium: number;
  thirdPartyRate: number;
  currency: string;
  id: number;
}

interface InsuranceState {
  results: PremiumResult[];
  exchangeRate: number;
  showForm: boolean;
  isFinished: boolean;
  calculatePremium: (data: Calculator) => Promise<void>;
  reset: () => void;
  setFinished: (value: boolean) => void;
  setShowForm: (value: boolean) => void;
  setExchangeRate: (value: number) => void;
}

export const useInsuranceStore = create < InsuranceState > ((set, get) => ({
  results: [],
  exchangeRate: 1,
  showForm: true,
  isFinished: false,

  calculatePremium: async (data: Calculator) => {
    try {
      const response = await apiRequest("POST", "/api/calculate", data);
      const result = await response.json();
      set((state) => ({
        ...state,
        results: [...state.results, result],
        showForm: false
      }));
    } catch (error) {
      console.error('Failed to calculate premium:', error);
      throw error;
    }
  },

  reset: () => set({
    results: [],
    showForm: true,
    isFinished: false,
    exchangeRate: 1
  }),

  setFinished: (value: boolean) => set({ isFinished: value }),
  setShowForm: (value: boolean) => set({ showForm: value }),
  setExchangeRate: (value: number) => set({ exchangeRate: value })
}));
