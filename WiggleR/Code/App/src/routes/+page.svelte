<script lang="ts">
	import TimelapseVideo from '../lib/TimelapseVideo.svelte';
	import ContoursAreaPlot from '../lib/plots/ContoursAreaPlot.svelte';
	import ContoursAreasPlot from '../lib/plots/ContoursAreasPlot.svelte';
	import ContoursAmountPlot from '../lib/plots/ContoursAmountPlot.svelte';
	import ContoursLocationsPlot from '../lib/plots/ContoursLocationsPlot.svelte';
</script>

<div class="layout">
	<div class="video">
		<TimelapseVideo />
	</div>
	<div class="plots">
		<ContoursLocationsPlot />
		<ContoursAreasPlot />
		<ContoursAreaPlot />
		<ContoursAmountPlot />
	</div>
</div>

<style>
	.video {
		background-color: var(--main-color);
		display: grid;
		justify-items: center;
		align-items: center;
	}
	.layout {
		display: grid;
		grid-template-columns: 1fr 1fr;
		gap: 10px;
		height: 100vh;
	}
	.plots {
		display: grid;
		grid-auto-rows: 50vh;
		overflow-y: scroll;
	}
</style>
